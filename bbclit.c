#include "bbclit.h"
#include <string.h>


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
