#ifndef BCLI_H
#define BCLI_H


// %2d;%2d;%2d where each %d can be color or style format
#define COLOR_FORMAT(a) char a[8]

/**
 * Dimensions
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
    struct __BOX* attached_boxes;
} BOX;


// CUSTOMIZE SETTINGS

struct CORNER
{
    char top_left, top_right, bottom_left, bottom_right;
};

struct BORDER
{
    char vertical, horizontal;
};

struct INTERSECTION
{
    char vertical_up, vertical_down, horizontal_right, horizontal_left, full_intersection;
};

struct COLOR
{
    COLOR_FORMAT(border);
    COLOR_FORMAT(text);
    COLOR_FORMAT(text2);
    COLOR_FORMAT(title);
};


typedef struct __CUSTOMIZE_SETTINGS
{
    struct CORNER corner;
    struct BORDER border;
    struct INTERSECTION intersection;
    struct COLOR color;

} CUSTOMIZE_SETTINGS;


extern CUSTOMIZE_SETTINGS DEFAULT_CUSTOMIZE_SETTINGS();


#endif // !BCLI_H
