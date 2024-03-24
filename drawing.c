#include "bbclit.h"
#include <wchar.h>
#include <locale.h>
#include <stdio.h>

#define IN_STREAM stdin
#define OUT_STREAM stdout




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


void draw_raw_box(DIMENSION global_size, struct CUSTOMIZE_SETTINGS settings)
{
    int p;

    cursor_goto(global_size.x0, global_size.y0);

    putwchar(settings.corner.top_left);
    for(p = global_size.x0; p < global_size.x1-1; p++)
        putwchar(settings.border.horizontal);
    putwchar(settings.corner.top_right);

    for(p = global_size.y0 + 1; p < global_size.y1; p++)
    {
        cursor_goto(global_size.x0, p);
        putwchar(settings.border.vertical);
        cursor_goto(global_size.x1, p);
        putwchar(settings.border.vertical);
    }

    cursor_goto(global_size.x0, global_size.y1);
    putwchar(settings.corner.bottom_left);
    for(p = global_size.x0; p < global_size.x1-1; p++)
        putwchar(settings.border.horizontal);
    putwchar(settings.corner.bottom_right);
    fflush(stdout);
}


void draw_box(BOX* self_box)
{
    DIMENSION global_size = relative_to_absolute_size(self_box);
    if(self_box->parent == NULL)
        draw_raw_box(global_size, self_box->settings);
}
