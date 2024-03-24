#include "bbclit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


void cursor_goto(int x, int y)
{
    wprintf(L"\e[%d;%dH", y, x);
}


void clear_screen()
{
    wprintf(L"\e[H\e[J");
    fflush(OUT_STREAM);
}


void __DEBUG_print_charId_matix(struct SCREEN* screen, DIMENSION global_size)
{
    cursor_goto(1, 1);
    for(int j = 0; j < global_size.y1; j++)
    {
        for(int i = 0; i < global_size.x1; i++)
            wprintf(L"%x", screen->screen_arr[i * (screen->size.y1) + j]);
    }
    fflush(OUT_STREAM);
}


void enable_wide_mode()
{
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
}


wchar_t get_character_by_id(struct CUSTOMIZE_SETTINGS settings, short id)
{
    switch(id)
    {
        case 3:
            return settings.corner.top_left;
        case 5:
            return settings.border.vertical;
        case 6:
            return settings.corner.bottom_left;
        case 7:
            return settings.intersection.vertical_left;
        case 9:
            return settings.corner.top_right;
        case 10:
            return settings.border.horizontal;
        case 11:
            return settings.intersection.horizontal_down;
        case 12:
            return settings.corner.bottom_right;
        case 13:
            return settings.intersection.vertical_right;
        case 14:
            return settings.intersection.horizontal_up;
        case 15:
            return settings.intersection.full_intersection;
        default:
            return L'\0';
    }
}


short get_intersection_id(short wcID1, short wcID2)
{
    /*
     * Intersections as
     *        2
     *   1    0    3
     *        4
     *  where numbers represent where characters are attached
     *  0 is default node
     *  Example: | is 2,4 -> 0101 -> 5
     *  Example: - is 1,3 -> 1010 -> 10
     *
     * To add intersections just do an or of their binary id number
     *
     */
    return wcID1 | wcID2;
}


short get_intersection_char(struct CUSTOMIZE_SETTINGS settings, short wcID1, short wcID2)
{
    /*
     * Intersections as
     *        2
     *   1    0    3
     *        4
     *  where numbers represent where characters are attached
     *  0 is default node
     *  Example: | is 2,4 -> 0101 -> 5
     *  Example: - is 1,3 -> 1010 -> 10
     *
     * To add intersections just do an or of their binary number
     *
     */
    return get_character_by_id(settings, get_intersection_id(wcID1, wcID2));
}

short get_character_id(struct CUSTOMIZE_SETTINGS settings, wchar_t c)
{
    if(c == settings.corner.top_left)
        return 3;
    if(c == settings.border.vertical)
        return 5;
    if(c == settings.corner.bottom_left)
        return 6;
    if(c == settings.intersection.vertical_right)
        return 7;
    if(c == settings.corner.top_right)
        return 9;
    if(c == settings.border.horizontal)
        return 10;
    if(c == settings.intersection.horizontal_down)
        return 11;
    if(c == settings.corner.bottom_right)
        return 12;
    if(c == settings.intersection.vertical_left)
        return 13;
    if(c == settings.intersection.horizontal_up)
        return 14;
    if(c == settings.intersection.full_intersection)
        return 15;
    else
    {
        wprintf(L"Cannot find character %lc!!!!!", c);
        exit(1);
        return L'\0';
    }
}


void draw_child_box(DIMENSION global_size, struct CUSTOMIZE_SETTINGS settings, struct SCREEN* screen)
{
    int p;
    wchar_t c;
    cursor_goto(global_size.x0, global_size.y0);
    putwchar(
    c = get_intersection_char(settings,
    get_character_id(settings, settings.corner.top_left),
    screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + global_size.y0 - 1]));
    screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + global_size.y0 - 1] =
    get_character_id(settings, c);
    for(p = global_size.x0 + 1; p < global_size.x1; p++)
    {
        putwchar(c = get_intersection_char(settings,
                 get_character_id(settings, settings.border.horizontal),
                 screen->screen_arr[(p - 1) * (screen->size.y1) + global_size.y0 - 1]));
        screen->screen_arr[(p - 1) * (screen->size.y1) + global_size.y0 - 1] =
        get_character_id(settings, c);
    }
    putwchar(
    c = get_intersection_char(settings,
    get_character_id(settings, settings.corner.top_right),
    screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + global_size.y0 - 1]));
    screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + global_size.y0 - 1] =
    get_character_id(settings, c);
    for(p = global_size.y0 + 1; p < global_size.y1; p++)
    {
        cursor_goto(global_size.x0, p);
        putwchar(c = get_intersection_char(settings,
                 get_character_id(settings, settings.border.vertical),
                 screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + p - 1]));
        screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + p - 1] =
        get_character_id(settings, c);
        cursor_goto(global_size.x1, p);
        putwchar(c = get_intersection_char(settings,
                 get_character_id(settings, settings.border.vertical),
                 screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + p - 1]));
        screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + p - 1] =
        get_character_id(settings, c);
    }
    cursor_goto(global_size.x0, global_size.y1);
    putwchar(
    c = get_intersection_char(settings,
    get_character_id(settings, settings.corner.bottom_left),
    screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + global_size.y1 - 1]));
    screen->screen_arr[(global_size.x0 - 1) * (screen->size.y1) + global_size.y1 - 1] =
    get_character_id(settings, c);
    for(p = global_size.x0 + 1; p < global_size.x1; p++)
    {
        putwchar(c = get_intersection_char(settings,
                 get_character_id(settings, settings.border.horizontal),
                 screen->screen_arr[(p - 1) * (screen->size.y1) + global_size.y1 - 1]));
        screen->screen_arr[(p - 1) * (screen->size.y1) + global_size.y1 - 1] =
        get_character_id(settings, c);
    }
    putwchar(
    c = get_intersection_char(settings,
    get_character_id(settings, settings.corner.bottom_right),
    screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + global_size.y1 - 1]));
    screen->screen_arr[(global_size.x1 - 1) * (screen->size.y1) + global_size.y1 - 1] =
    get_character_id(settings, c);
    fflush(OUT_STREAM);
}


void draw_box(BOX* self_box, struct SCREEN* screen)
{
    DIMENSION global_size = (self_box->size);
    // check if dimension is possible
    draw_child_box(global_size, self_box->settings, screen);
}
