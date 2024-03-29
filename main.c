/*
 * This file is used to test the library
 * Creator: Hugo Coto Florez
 */

#include "bbclit.h"
#include <stdbool.h>

BOX split1, split2, split3;
bool isignal = false;



void trigger(int n)
{
    switch(n)
    {
        case 1:
            __LOGPRINT("-- trigger -- 1", __LINE__);
            appendnl_text("Entry 1 selected");
            break;
        case 2:
            __LOGPRINT("-- trigger -- 2", __LINE__);
            appendnl_text("Entry 2 selected");
            break;
        default:
            __LOGPRINT("-- trigger -- n", __LINE__);
            appendnl_text("Getting text");
            char str[MAX_LINE_LEN];
            get_input(split3, str, MAX_LINE_LEN, &isignal);
            appendnl_text(str);
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
    template1(&split1, &split2, &split3);

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

    keyboard_handler(&isignal);

    clear_screen();
    show_cursor();

    return 0;
}
