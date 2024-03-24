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
    struct SCREEN screen = initialize_screen();
    DIMENSION size = fullscreen();
    enable_wide_mode();
    BOX main_box = new_box(NULL, size);
    draw_box(&main_box, &screen);
    bool i = false;
    keyboard_handler(i);
    free(screen.screen_arr);


    return 0;
}
