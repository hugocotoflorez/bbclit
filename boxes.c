#include "bbclit.h"
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>


struct CUSTOMIZE_SETTINGS DEFAULT_CUSTOMIZE_SETTINGS()
{
    struct CUSTOMIZE_SETTINGS default_settings;
    default_settings.corner.top_left                = L'┏';
    default_settings.corner.top_right               = L'┓';
    default_settings.corner.bottom_left             = L'┗';
    default_settings.corner.bottom_right            = L'┛';
    default_settings.border.vertical                = L'┃';
    default_settings.border.horizontal              = L'━';
    default_settings.intersection.vertical_left     = L'┣';
    default_settings.intersection.vertical_right    = L'┫';
    default_settings.intersection.horizontal_up     = L'┻';
    default_settings.intersection.horizontal_down   = L'┳';
    default_settings.intersection.full_intersection = L'╋';
    strcpy(default_settings.color.border, "\e[0;37m");
    strcpy(default_settings.color.text, "\e[0;1;34m");
    strcpy(default_settings.color.text2, "\e[0;1;30;44m");
    strcpy(default_settings.color.title, "\e[0;1;3;32m");
    return default_settings;
}


BOX new_box(DIMENSION dimension)
{
    BOX new_box;
    new_box.size     = dimension;
    new_box.settings = DEFAULT_CUSTOMIZE_SETTINGS();
    return new_box;
}


DIMENSION fullscreen()
{
    DIMENSION size;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    size.x0 = 1;
    size.y0 = 1;
    size.x1 = w.ws_col;
    size.y1 = w.ws_row;
    return size;
}


struct SCREEN initialize_screen()
{
    struct SCREEN screen;
    screen.size = fullscreen();
    screen.screen_arr = malloc(sizeof(short) * (screen.size.y1) * (screen.size.x1));
    for(int i = 0; i < (screen.size.y1 - 1) * (screen.size.x1 - 1); i++)
        screen.screen_arr[i] = 0;
    return screen;
}


void vsplit(DIMENSION src_size, DIMENSION* dest_size_top, DIMENSION* dest_size_bottom, float redux)
{
    // if redux > 1 or redux too litle may cause error
    *dest_size_top =
    (DIMENSION){ src_size.x0, src_size.x1, src_size.y0, src_size.y1 * redux };
    *dest_size_bottom =
    (DIMENSION){ src_size.x0, src_size.x1, src_size.y1 * redux, src_size.y1 };
}


void hsplit(DIMENSION src_size, DIMENSION* dest_size_left, DIMENSION* dest_size_right, float redux)
{
    // if redux > 1 or redux too litle may cause error
    *dest_size_left =
    (DIMENSION){ src_size.x0, src_size.x1*redux, src_size.y0, src_size.y1 };
    *dest_size_right =
    (DIMENSION){ src_size.x1*redux, src_size.x1, src_size.y0, src_size.y1 };
}
