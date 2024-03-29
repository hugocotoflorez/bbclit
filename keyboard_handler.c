/*
 * Independent keyboard handler
 *
 * Raw mode source code:
 * (Documentation about RawMode here)
 * https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 *
 * Authors:
 * Hugo Coto Florez
 */

#include "bbclit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#define EXIT_POINT 27 // ESC key


struct termios origin_termios;


typedef struct __BIND
{
    void (*action)(void);
    char key;
    bool active;
} BIND;


struct __KEYBINDS
{
    bool (*key_exists)(char);
    void (*add)(char, void (*action)(void));
    BIND* binds;
    int binds_n;
} KEYBINDS;


bool key_exists(char key)
{
    for(int i = 0; i < KEYBINDS.binds_n; i++)
    {
        if(KEYBINDS.binds[i].key == key)
            return true;
    }
    return false;
}


void add(char key, void (*action)(void))
{
    KEYBINDS.binds = realloc(KEYBINDS.binds, (KEYBINDS.binds_n + 1) * sizeof(BIND));
    KEYBINDS.binds[KEYBINDS.binds_n++] = (BIND){ action, key };
}


void free_binds()
{
    if(KEYBINDS.binds)
        free(KEYBINDS.binds);
}


void delete_keybinds()
{
    free_binds();
    KEYBINDS.binds   = NULL;
    KEYBINDS.binds_n = 0;
}


void initialize_keybinds()
{
    KEYBINDS.key_exists = key_exists;
    KEYBINDS.add        = add;
    KEYBINDS.binds      = NULL;
    KEYBINDS.binds_n    = 0;
    atexit(delete_keybinds);
}


void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}


void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &origin_termios);
    atexit(disableRawMode);
    struct termios raw;
    raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_lflag &= ~(OPOST);
    raw.c_lflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    // raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


// should work but..
void mssleep(int milliseconds)
{
    struct timespec t;
    struct timespec _t;
    t.tv_sec  = milliseconds / 1000;
    t.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&t, &_t);
}


void execute_bind(char key)
{
    for(int i = 0; i < KEYBINDS.binds_n; i++)
    {
        if(KEYBINDS.binds[i].key == key)
        {
            if(KEYBINDS.binds[i].active)
                KEYBINDS.binds[i].action();
            return;
        }
    }
}


void set_binds_active()
{
    for(int i = 0; i < KEYBINDS.binds_n; i++)
    {
        KEYBINDS.binds[i].active = true;
    }
}

void set_binds_inactive()
{
    for(int i = 0; i < KEYBINDS.binds_n; i++)
    {
        KEYBINDS.binds[i].active = false;
    }
}

/**
 * bind
 * Bind a function to a key
 * 1) if key is not binded it creates a new bind
 * 2) if key is binded it replace the function with the new one
 *
 * Isnt needed to unbind keys at exit
 */
void bind(char key, void f(void))
{
    for(int i = 0; i < KEYBINDS.binds_n; i++)
        if(KEYBINDS.binds[i].key == key)
        {
            KEYBINDS.binds[i].action = f;
            KEYBINDS.binds[i].active = true;
            return;
        }
    KEYBINDS.add(key, f);
}

bool IS_HANDLER_ACTIVE = false;

/**
 * keyboard_handler
 * binds are only active if keyboard_handler is running.
 * this behaviour can be changed manually (not recommended)
 */
void keyboard_handler(bool* CANCELLATION_SIGNAL)
{
    //__LOGPRINT("INTO KBDH", __LINE__);
    if(IS_HANDLER_ACTIVE)
    {
        //__LOGPRINT("ERROR HANDLER ALREADY ACTIVE", __LINE__);
        return;
    }

    char c = 0;
    // char s[30]; // IDK WHAT THE FUCK HAPPENED BUT WITHOUT THIS
    //  DECLARATION IT EXPLOTES AND execute_bind DONT
    //  EXECUTES enter KEYPRESSES
    /*
     * Looks like moving o from int to ssize_t fixed the problem
     * I think that some overflow in o writes c and because of this
     * c is not recognised corectly by execute_bind
     *
     * Althought, im not going to end this issue after make sure is totally fixed
     */
    ssize_t o;
    enableRawMode();
    set_binds_active();
    while((o = read(STDIN_FILENO, &c, 1)) >= 0 && c != EXIT_POINT)
    {
        if(*CANCELLATION_SIGNAL)
        {
            //__LOGPRINT("Exiting khandler", __LINE__);
            break;
        }
        if(o == 0) // si no hay input
        {
            mssleep(100);
        }
        else
        {
            // sprintf(s, "character (%d) pressed", c);
            // appendnl_text(s);
            execute_bind(c);
            c = 0; // avoid repetitive calls to last character
        }
    }
    disableRawMode();
    set_binds_inactive();
    IS_HANDLER_ACTIVE = false;
}


/**
 * input_string
 * FILE* IN_STREAM: STDIN or custom input stream file
 * char* OUT_STR: string where input will be copied
 * int MAX_LEN: maximum length of OUT_STR
 *
 * Return:
 *  false (0): success
 *  true  (1): error
 *
 * ! Entry in cooked mode and dont come back raw
 */
bool input_string(FILE* IN_STREAM, char* OUT_STR, int MAX_LEN, int MAX_OUTPUT_LEN)
{
    char c;
    ssize_t o;
    char s[40];
    for(int i = 0; i < MAX_LEN; i++)
        if((o = read(STDIN_FILENO, &c, 1)) >= 0)
        {
            if(o == 0) // si no hay input
            {
                mssleep(100);
                i--;
                continue;
            }
            if(c == 13)
            {
                sprintf(s, "\\0 into s[%d]", i);
                __LOGPRINT(s, __LINE__);
                OUT_STR[i] = '\0';
                break;
            }
            if(i < MAX_OUTPUT_LEN)
            {
                putwchar(c);
                fflush(OUT_STREAM);
            }
            OUT_STR[i] = c;
            c          = 0;
            sprintf(s, "Insert %c(%d) into s[%d]", c, c, i);
            __LOGPRINT(s, __LINE__);
        }
        else
            return false;

    __LOGPRINT(OUT_STR, __LINE__);

    return true;
}
