#ifndef BCLI_H
#define BCLI_H


// \e[0;%2d;%2d;%2dm where each %d can be color or style format
#define COLOR_FORMAT(a) char a[13]

#define LARGE_CHAR(a) char a[2]

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
    LARGE_CHAR(top_left);
    LARGE_CHAR(top_right);
    LARGE_CHAR(bottom_left);
    LARGE_CHAR(bottom_right);
};

struct BORDER
{
    LARGE_CHAR(vertical);
    LARGE_CHAR(horizontal);
};

struct INTERSECTION
{
    LARGE_CHAR(vertical_right);
    LARGE_CHAR(vertical_left);
    LARGE_CHAR(horizontal_up);
    LARGE_CHAR(horizontal_down);
    LARGE_CHAR(full_intersection);
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


CUSTOMIZE_SETTINGS DEFAULT_CUSTOMIZE_SETTINGS();


#endif // !BCLI_H
