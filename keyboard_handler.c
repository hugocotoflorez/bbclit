/*
 * Independent keyboard hadler
 *
 * Raw mode source code:
 * (Documentation about RawMode here)
 * https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 *
 * Authors:
 * Hugo Coto Florez
 */

#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define EXIT_POINT 27 // ESC key

struct termios origin_termios;


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
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/**
 * keyboard_handler
 */
//possible declaration
//void keyboard_handler(int* buffer, int* current_buffer_position, void(*f)(void) launcher_on_keypress)
void keyboard_handler()
{
    char c;
    enableRawMode();
    while(read(STDIN_FILENO, &c, 1) && c != EXIT_POINT)
    {
        // TODO
    }
    disableRawMode();
}



