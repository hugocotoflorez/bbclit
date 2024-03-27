#include "bbclit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


struct __ACTIVE_WINDOW
{
    DIMENSION current_ptr;
    DIMENSION size;
    struct CUSTOMIZE_SETTINGS settigns;
    MODULE_OPTIONS options;
    void (*trigger)(int);
} ACTIVE_WINDOW;


/**
 * DEFAULT\_MODULE\_OPTIONS
 *
 * Return MODULE\_OPTIONS with default settings
 */
MODULE_OPTIONS DEFAULT_MODULE_OPTIONS()
{
    MODULE_OPTIONS options;
    options.align     = RIGHT;
    options.options   = NULL;
    options.actions_n = 0;
    return options;
}


/**
 * putline
 *
 * Print the line number 'line' in ACTIVE\_WINDOW.
 * It dont move the cursor, apply color or any formatting
 */
void putline(unsigned int line)
{
    struct MODULE_ACTIONS actions;
    if(line < ACTIVE_WINDOW.options.actions_n)
    {
        actions = ACTIVE_WINDOW.options.options[line];
        wprintf(L"%c) %s", actions.caller, actions.text_msg);
        fflush(OUT_STREAM);
    }
}

void apply_color(COLOR_FORMAT(c))
{
    // Color should be in ACTIVE_WINDOW.settigns.color
    wprintf(L"%s", c);
}


void select_down()
{
    /*
     * -- STEPS --
     * Move the cursor to the current selected line
     * rewrite this line without highlighting
     * Move the pointer and the cursor one line below
     * Rewrite this line with the highlight color
     *
     * Active Window MUST be the selection menu this
     * function is attached. current_ptr MUST point to the
     * highlighted line
     *
     * ! This function move the pointer.
     */
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    apply_color(ACTIVE_WINDOW.settigns.color.text);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
    ACTIVE_WINDOW.current_ptr.y0 %= ACTIVE_WINDOW.options.actions_n;
    ACTIVE_WINDOW.current_ptr.y0++;
    apply_color(ACTIVE_WINDOW.settigns.color.text2); // highlight color
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
}


void select_up()
{
    /*
     * -- STEPS --
     * Move the cursor to the current selected line
     * rewrite this line without highlighting
     * Move the pointer and the cursor one line above
     * Rewrite this line with the highlight color
     *
     * Active Window MUST be the selection menu this
     * function is attached. current_ptr MUST point to the
     * highlighted line
     *
     * ! This function move the pointer.
     */
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    apply_color(ACTIVE_WINDOW.settigns.color.text);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
    ACTIVE_WINDOW.current_ptr.y0--;
    if(ACTIVE_WINDOW.current_ptr.y0 == 0)
        ACTIVE_WINDOW.current_ptr.y0 = ACTIVE_WINDOW.options.actions_n;
    apply_color(ACTIVE_WINDOW.settigns.color.text2);
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
}


void execute()
{
    // BINDED FUNCTION
    // can be use as a trigger
    ACTIVE_WINDOW.trigger(ACTIVE_WINDOW.current_ptr.y0);
    fflush(OUT_STREAM);
}


void build_selection()
{
    /*
     * Print all the menu entries except the first one with
     * default text color. Then print the first one with
     * highlight color and set the cursor in the first entry
     */
    apply_color(ACTIVE_WINDOW.settigns.color.text);
    for(int i = 1; i < ACTIVE_WINDOW.options.actions_n; i++)
    {
        cursor_goto(ACTIVE_WINDOW.size.x0 + 1, ACTIVE_WINDOW.size.y0 + 1 + i);
        putline(i);
    }
    ACTIVE_WINDOW.current_ptr.y0 = 1;
    apply_color(ACTIVE_WINDOW.settigns.color.text2);
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    putline(0);
}


/**
 * selection_box
 *
 * DEFAULTS
 *  - j: move down
 *  - k: move up
 *
 *  void trigger(int) is called when enter key
 *  is pressed. Arguments are [1, n], where 1 is the
 *  first (top) entry and n is the n`th entry (bottom)
 *
 *  Is important not to bind 'j' or 'k' manually for other purpose.
 *  Calling this function again just rebinds the keys
 *  and restore ACTIVE_WINDOW settigns.
 */
void selection_box(void f(int), BOX parent, MODULE_OPTIONS opt)
{
    ACTIVE_WINDOW.size        = parent.size;
    ACTIVE_WINDOW.settigns    = parent.settings;
    ACTIVE_WINDOW.current_ptr = parent.box_ptr;
    ACTIVE_WINDOW.options     = opt;
    ACTIVE_WINDOW.trigger     = f;
    bind('j', select_down);
    bind('k', select_up);
    bind(13, execute); // run 'execute' on enter (13) press
}


void add_entry(void func(void), char caller, const char* text)
{
    //__printlog("Add entry", __LINE__);
    ACTIVE_WINDOW.options.options = realloc(ACTIVE_WINDOW.options.options,
    sizeof(struct MODULE_ACTIONS) * (ACTIVE_WINDOW.options.actions_n + 1));
    ACTIVE_WINDOW.options.options[ACTIVE_WINDOW.options.actions_n].caller = caller;
    ACTIVE_WINDOW.options.options[ACTIVE_WINDOW.options.actions_n].action = func;
    strncpy(ACTIVE_WINDOW.options.options[ACTIVE_WINDOW.options.actions_n].text_msg,
    text, MAX_LINE_LEN);
    ACTIVE_WINDOW.options.actions_n++;
    build_selection(); // esto era mejor ponerlo en otro sitio
}
