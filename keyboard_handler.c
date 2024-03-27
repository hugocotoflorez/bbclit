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
    free(KEYBINDS.binds);
}


void delete_keybinds()
{
    if(KEYBINDS.binds != NULL)
        free(KEYBINDS.binds);
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
    // raw.c_cc[VTIME] = 0;
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
            KEYBINDS.binds[i].action();
            return;
        }
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
            return;
        }
    KEYBINDS.add(key, f);
}


/**
 * keyboard_handler
 */
void keyboard_handler(bool CANCELLATION_SIGNAL)
{
    char c = '#';
    int o;
    enableRawMode();
    // TODO: ERROR? if waiting for input and then CANCELLATION_SIGNAL is called it wouldn exit until read refreshes
    while((o = read(STDIN_FILENO, &c, 1)) >= 0 && c != EXIT_POINT && !CANCELLATION_SIGNAL)
    {
        // HAY DELAY, SE PUEDE BAJAR EL SLEEP
        if(o == 0) // si no hay input
        {
            // wprintf(L"\e[11;11H SP");
            //  idk why but this sleep for 1/10 s
            mssleep(10);
        }
        execute_bind(c);
        // wprintf(L"\e[10;10H%c", c);
        //  wprintf(L"\e[11;11H UP");
    }
    disableRawMode();
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
bool input_string(FILE* IN_STREAM, char* OUT_STR, int MAX_LEN)
{
    disableRawMode();
    return fgets(OUT_STR, MAX_LEN, IN_STREAM) == NULL;
}
