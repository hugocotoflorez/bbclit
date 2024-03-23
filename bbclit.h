#ifndef BCLI_H
#define BCLI_H


typedef struct __DIMENSIONS
{
    int x0, x1, y0, y1;
} DIMENSIONS;


typedef struct __BOX
{
    DIMENSIONS size;
    struct __BOX* attached_boxes;
} BOX;


#endif // !BCLI_H
