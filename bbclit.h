#ifndef BCLI_H
#define BCLI_H

#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
// .......... DEFINES
#define IN_STREAM stdin
#define OUT_STREAM stdout

// \e[0;%2d;%2d;%2dm where each %d can be color or style format
#define COLOR_FORMAT(a) char a[13]

// .......... CUSTOMIZE SETTINGS

struct CORNER
{
    wchar_t top_left;
    wchar_t top_right;
    wchar_t bottom_left;
    wchar_t bottom_right;
};

struct BORDER
{
    wchar_t vertical;
    wchar_t horizontal;
};

struct INTERSECTION
{
    wchar_t vertical_right;
    wchar_t vertical_left;
    wchar_t horizontal_up;
    wchar_t horizontal_down;
    wchar_t full_intersection;
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
    struct CUSTOMIZE_SETTINGS settings;
} BOX;


// .......... SCREEN MATRIX
struct SCREEN
{
    DIMENSION size;
    short* screen_arr;
};

// FUNCTIONS

extern DIMENSION fullscreen();
extern BOX new_box(BOX* parent, DIMENSION size);
extern void draw_box(BOX* self_box, struct SCREEN* screen);
extern void clear_screen();
extern void keyboard_handler(bool CANCELLATION_SIGNAL);
extern void enable_wide_mode();
extern struct SCREEN initialize_screen();
#endif // !BCLI_H
