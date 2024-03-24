/*
 * This file is used to test the library
 * Creator: Hugo Coto Florez
 */

#include "bbclit.h"
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int main(int argc, char** arcv)
{
    // clear_screen();
    DIMENSION size = fullscreen();
    // DIMENSION size = {5,10,5,10};
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
    BOX main_box = new_box(NULL, size);
    draw_box(&main_box);
    bool i = false;
    keyboard_handler(i);

    return 0;
}
