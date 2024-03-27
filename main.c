/*
 * This file is used to test the library
 * Creator: Hugo Coto Florez
 */

#include "bbclit.h"
#include <stdbool.h>

void __LOG_PRINT(char* msg, int n)
{
    FILE* f = fopen("log.txt", "a");
    fprintf(f, "[%s] (%d):\t%s\n", __TIME__, n, msg);
    fclose(f);
}

void trigger(int n)
{
    switch(n)
    {
        case 1:
            //__LOG_PRINT("-- trigger -- 1", __LINE__);
            appendnl_text("Entry 1 selected");
            break;
        case 2:
            //__LOG_PRINT("-- trigger -- 2", __LINE__);
            appendnl_text("Entry 2 selected");
            break;
        default:
            //__LOG_PRINT("-- trigger -- n", __LINE__);
            appendnl_text("Some entry selected");
            break;
    }
}


int main(int argc, char** arcv)
{
    enable_wide_mode();
    hide_cursor();
    clear_screen();
    initialize_keybinds();
    MODULE_OPTIONS opt   = DEFAULT_MODULE_OPTIONS();
    struct SCREEN screen = initialize_screen();
    /*
     * IDK WHY BUT IF I USE THE SAME DIMENSION AS SRC AND DEST
     * SOMETIMES IT CRASHES AND PRINT THE BORDERS WRONGLY. IF
     * I USE SIZE AS DEST IT CRASHES AND SOME PART OF THE SCREEN
     * STOPS WORKING.       TODO! FIX THAT SHIT
     */
    DIMENSION size = fullscreen(); // no used, just to create splits
    DIMENSION size1;               // no used
    DIMENSION size2;               // no used
    DIMENSION size3;

    vsplit(size, &size1, &size2, 0.25);
    hsplit(size1, &size1, &size3, 0.75);

    BOX split1 = new_box(size1);
    BOX split2 = new_box(size2);
    BOX split3 = new_box(size3);

    draw_box(&split1, &screen);
    draw_box(&split2, &screen);
    draw_box(&split3, &screen);

    initialize_paragraph(split2);

    selection_box(trigger, split1, opt);
    add_entry(0, '1', "Entry 1");
    add_entry(0, '2', "Entry 2");
    add_entry(0, '3', "Entry 3");
    add_entry(0, '4', "Entry 4");
    add_entry(0, '5', "Entry 5");

    bool i = false;
    keyboard_handler(i);

    clear_screen();
    show_cursor();

    return 0;
}
