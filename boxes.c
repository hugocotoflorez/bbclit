#include "bbclit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>


BOX new_box(BOX* parent, DIMENSION dimension)
{
    BOX new_box;
    new_box.parent           = parent;
    new_box.size             = dimension;
    new_box.settings         = DEFAULT_CUSTOMIZE_SETTINGS();
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
