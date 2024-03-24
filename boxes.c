#include "bbclit.h"
#include <stdlib.h>
#include <sys/ioctl.h>


BOX new_box(BOX* parent, DIMENSION dimension)
{
    BOX new_box;
    new_box.parent   = parent;
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
