#ifndef BCLI_H
#define BCLI_H

#include <wchar.h>
#include <stdbool.h>
// .......... DEFINES

// \e[0;%2d;%2d;%2dm where each %d can be color or style format
#define COLOR_FORMAT(a) char a[13]


// .......... CUSTOMIZE SETTINGS

struct CORNER
{
    const wchar_t top_left;
    const wchar_t top_right;
    const wchar_t bottom_left;
    const wchar_t bottom_right;
};

struct BORDER
{
    const wchar_t vertical;
    const wchar_t horizontal;
};

struct INTERSECTION
{
    const wchar_t vertical_right;
    const wchar_t vertical_left;
    const wchar_t horizontal_up;
    const wchar_t horizontal_down;
    const wchar_t full_intersection;
};

struct COLOR
{
    COLOR_FORMAT(border);
    COLOR_FORMAT(text);
    COLOR_FORMAT(text2);
    COLOR_FORMAT(title);
};


struct CUSTOMIZE_SETTINGS
{
    struct CORNER corner;
    struct BORDER border;
    struct INTERSECTION intersection;
    struct COLOR color;
};


// .......... BOXES

/**
 * Dimensions
 * start in x0,y0 (included) and exit in x1,y1 (excluded)
 * X is horizontal axis
 * Y is vertical axis
 */
typedef struct __DIMENSION
{
    int x0, x1, y0, y1;
} DIMENSION;


typedef struct __BOX
{
    DIMENSION size;
    struct __BOX* parent;
    struct CUSTOMIZE_SETTINGS settings;
} BOX;


// FUNCTIONS

extern struct CUSTOMIZE_SETTINGS DEFAULT_CUSTOMIZE_SETTINGS();
extern BOX new_box(BOX* parent, DIMENSION size);
extern DIMENSION fullscreen();
extern void draw_box(BOX* self_box);
extern void clear_screen();
extern void keyboard_handler(bool CANCELLATION_SIGNAL);
extern void enable_wide_mode();
#endif // !BCLI_H
