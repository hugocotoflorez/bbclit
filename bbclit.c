#include "bbclit.h"
#include <string.h>

#define ASSIGN(dest, src)  \
    {                      \
        strcpy(dest, src); \
    }


CUSTOMIZE_SETTINGS DEFAULT_CUSTOMIZE_SETTINGS()
{
    CUSTOMIZE_SETTINGS default_settings;
    // corners
    ASSIGN(default_settings.corner.top_left, "┏");
    ASSIGN(default_settings.corner.top_right, "┓");
    ASSIGN(default_settings.corner.bottom_left, "┗");
    ASSIGN(default_settings.corner.bottom_right, "┛");
    // borders
    ASSIGN(default_settings.border.vertical, "┃");
    ASSIGN(default_settings.border.horizontal, "━");
    // intersection
    ASSIGN(default_settings.intersection.vertical_left, "┣");
    ASSIGN(default_settings.intersection.vertical_right, "┫");
    ASSIGN(default_settings.intersection.horizontal_up, "┻");
    ASSIGN(default_settings.intersection.horizontal_down, "┳");
    ASSIGN(default_settings.intersection.full_intersection, "╋");
    // color
    ASSIGN(default_settings.color.border, "\e[0;37m");
    ASSIGN(default_settings.color.text, "\e[0;1;34m");
    ASSIGN(default_settings.color.text2, "\e[0;1;30;44m");
    ASSIGN(default_settings.color.title, "\e[0;1;3;32m");


    return default_settings;
}
