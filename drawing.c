#include "bbclit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


void enable_wide_mode()
{
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
}


void clear_screen()
{
    wprintf(L"\e[2J");
}


void cursor_goto(int x, int y)
{
    wprintf(L"\e[%d;%dH", y, x);
}


DIMENSION relative_to_absolute_size(BOX* self_box)
{
    DIMENSION absolute_size = { 0, 0, 0, 0 };
    for(BOX* current = self_box; current != NULL; current = current->parent)
    {
        absolute_size.x0 += current->size.x0;
        absolute_size.y0 += current->size.y0;
        absolute_size.x1 += current->size.x1;
        absolute_size.y1 += current->size.y1;
    }
    return absolute_size;
}


short get_intersection(short c1, short c2)
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
    return c1 | c2;
}


wchar_t get_character_by_id(struct CUSTOMIZE_SETTINGS settings, short id)
{
    switch(id)
    {
        case 3:
            return settings.corner.top_left;
            break;
        case 5:
            return settings.border.vertical;
            break;
        case 6:
            return settings.corner.bottom_left;
            break;
        case 7:
            return settings.intersection.vertical_right;
            break;
        case 9:
            return settings.corner.top_right;
            break;
        case 10:
            return settings.border.horizontal;
            break;
        case 11:
            return settings.intersection.horizontal_down;
            break;
        case 12:
            return settings.corner.bottom_right;
            break;
        case 13:
            return settings.intersection.vertical_left;
            break;
        case 14:
            return settings.intersection.horizontal_up;
            break;
        case 15:
            return settings.intersection.full_intersection;
            break;
        default:
            wprintf(L"Cannot find %d character!!!!!", id);
            return L'\0';
    }
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
        return L'\0';
    }
}


void draw_raw_box(DIMENSION global_size, struct CUSTOMIZE_SETTINGS settings, struct SCREEN* screen)
{
    int p;
    cursor_goto(global_size.x0, global_size.y0);
    putwchar(settings.corner.top_left);
    {
        // TODO: RESTARLE 1 a todo
        screen->screen_arr[(global_size.x0-1) * (screen->size.y1 - 1) + global_size.y0-1] =
        get_character_id(settings, settings.corner.top_left);
    }
    for(p = global_size.x0 + 1; p < global_size.x1; p++)
    {
        putwchar(settings.border.horizontal);
        {
            screen->screen_arr[(global_size.x0 + p) * (screen->size.y1 - 1) +
            global_size.y0] = get_character_id(settings, settings.corner.top_left);
        }
    }
    putwchar(settings.corner.top_right);
    {
        screen
        ->screen_arr[(global_size.x1) * (screen->size.y1 - 1) + global_size.y0] =
        get_character_id(settings, settings.corner.top_left);
    }
    for(p = global_size.y0 + 1; p < global_size.y1; p++)
    {
        cursor_goto(global_size.x0, p);
        putwchar(settings.border.vertical);
        {
            screen->screen_arr[(global_size.x0) * (screen->size.y1 - 1) + p] =
            get_character_id(settings, settings.corner.top_left);
        }
        cursor_goto(global_size.x1, p);
        putwchar(settings.border.vertical);
        {
            screen->screen_arr[(global_size.x1) * (screen->size.y1 - 1) + p] =
            get_character_id(settings, settings.corner.top_left);
        }
    }
    cursor_goto(global_size.x0, global_size.y1);
    putwchar(settings.corner.bottom_left);
    {
        screen
        ->screen_arr[(global_size.x0) * (screen->size.y1 - 1) + global_size.y1] =
        get_character_id(settings, settings.corner.top_left);
    }
    for(p = global_size.x0 + 1; p < global_size.x1; p++)
    {
        putwchar(settings.border.horizontal);
        {
            screen->screen_arr[(global_size.x0 + p) * (screen->size.y1 - 1) +
            global_size.y1] = get_character_id(settings, settings.corner.top_left);
        }
    }
    putwchar(settings.corner.bottom_right);
    {
        screen
        ->screen_arr[(global_size.x1) * (screen->size.y1 - 1) + global_size.y1] =
        get_character_id(settings, settings.corner.top_left);
    }
    fflush(OUT_STREAM);
    cursor_goto(1, 1);
    for(int i = 0; i < global_size.x1; i++)
        for(int j = 0; j < global_size.y1; j++)
            wprintf(L"%d", screen->screen_arr[j * (screen->size.y1 - 1) + i]);
}


void draw_child_box()
{
}


void draw_box(BOX* self_box, struct SCREEN* screen)
{
    DIMENSION global_size = relative_to_absolute_size(self_box);
    // check if dimension is possible
    if(self_box->parent == NULL)
        draw_raw_box(global_size, self_box->settings, screen);
    else
        draw_child_box();
}
