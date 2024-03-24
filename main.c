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
    enable_wide_mode();
    clear_screen();
    struct SCREEN screen = initialize_screen();
    DIMENSION size       = fullscreen();
    DIMENSION size2      = { size.x0, size.x1 / 2, size.y0, size.y1 };
    DIMENSION size3      = { size2.x0, size2.x1 / 3, size2.y0, size2.y1 };
    DIMENSION size4      = { size2.x0+10, size2.x1-10, size2.y0 + 10, size2.y1 };
    BOX main_box         = new_box(NULL, size);
    BOX split            = new_box(NULL, size2);
    BOX split2           = new_box(NULL, size3);
    BOX split3           = new_box(NULL, size4);

    draw_box(&main_box, &screen);
    draw_box(&split, &screen);
    draw_box(&split2, &screen);
    draw_box(&split3, &screen);
    bool i = false;
    keyboard_handler(i);
    free(screen.screen_arr);
    clear_screen();
    return 0;
}
