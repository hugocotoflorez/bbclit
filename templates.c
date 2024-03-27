#include "bbclit.h"


void template1(BOX* split1, BOX* split2, BOX* split3)
{
    DIMENSION size = fullscreen();
    DIMENSION size1;
    DIMENSION size2;
    DIMENSION size3;
    DIMENSION size4;

    vsplit(size, &size1, &size2, 0.25);
    hsplit(size1, &size3, &size4, 0.75);

    *split1 = new_box(size3);
    *split2 = new_box(size2);
    *split3 = new_box(size4);
}
