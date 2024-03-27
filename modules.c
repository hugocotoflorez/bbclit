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
} ACTIVE_WINDOW;


MODULE_OPTIONS DEFAULT_MODULE_OPTIONS()
{
    MODULE_OPTIONS options;
    options.align     = RIGHT;
    options.options   = NULL;
    options.actions_n = 0;
    return options;
}


// print the line number 'line' in ACTIVE_WINDOW
// It dont move the cursor, apply color or any formatting
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
    wprintf(L"%s", c);
}


void select_down()
{
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
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    apply_color(ACTIVE_WINDOW.settigns.color.text);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
    ACTIVE_WINDOW.current_ptr.y0--;
    if(ACTIVE_WINDOW.current_ptr.y0 == 0)
        ACTIVE_WINDOW.current_ptr.y0 = ACTIVE_WINDOW.options.actions_n;
    apply_color(ACTIVE_WINDOW.settigns.color.text2); // highlight color
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    putline(ACTIVE_WINDOW.current_ptr.y0 - 1);
}


void execute()
{
    wprintf(L"\e[10;10H%d", ACTIVE_WINDOW.current_ptr.y0);
    fflush(OUT_STREAM);
}


void build_selection()
{
    apply_color(ACTIVE_WINDOW.settigns.color.text);
    for(int i = 1; i < ACTIVE_WINDOW.options.actions_n; i++)
    {
        cursor_goto(ACTIVE_WINDOW.size.x0 + 1, ACTIVE_WINDOW.size.y0 + 1 + i);
        putline(i);
    }
    ACTIVE_WINDOW.current_ptr.y0 = 1;                // reset to top
    apply_color(ACTIVE_WINDOW.settigns.color.text2); // highlight color
    cursor_goto(ACTIVE_WINDOW.size.x0 + 1,
    ACTIVE_WINDOW.size.y0 + ACTIVE_WINDOW.current_ptr.y0);
    putline(0);
}


void selection_box(BOX parent, MODULE_OPTIONS opt)
{
    //__printlog("-------------", __LINE__);
    //__printlog("selection box", __LINE__);
    ACTIVE_WINDOW.size        = parent.size;
    ACTIVE_WINDOW.settigns    = parent.settings;
    ACTIVE_WINDOW.current_ptr = parent.box_ptr;
    ACTIVE_WINDOW.options     = opt;
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
