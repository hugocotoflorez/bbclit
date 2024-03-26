#include "bbclit.h"
#include <stdlib.h>
#include <string.h>


struct __ACTIVE_WINDOW
{
    int current_x;
    int current_y;
    DIMENSION size;
    struct CUSTOMIZE_SETTINGS settigns;
} ACTIVE_WINDOW;


MODULE_OPTIONS DEFAULT_MODULE_OPTIONS()
{
    MODULE_OPTIONS options;
    options.align     = RIGHT;
    options.options   = NULL;
    options.actions_n = 0;
    return options;
}


void select_hilight()
{
}

void select_unhighlight()
{
}


void select_down()
{
    select_unhighlight();
    ACTIVE_WINDOW.current_y =
    (ACTIVE_WINDOW.current_y + 1) % (ACTIVE_WINDOW.size.y1 - ACTIVE_WINDOW.size.y0);
    select_hilight();
}

void select_up()
{
    select_unhighlight();
    ACTIVE_WINDOW.current_y =
    (ACTIVE_WINDOW.current_y - 1) % (ACTIVE_WINDOW.size.y1 - ACTIVE_WINDOW.size.y0);
    select_hilight();
}

void execute()
{

}

void selection_box(BOX parent, MODULE_OPTIONS opt)
{
    ACTIVE_WINDOW.size      = parent.size;
    ACTIVE_WINDOW.settigns  = parent.settings;
    ACTIVE_WINDOW.current_y = parent.size.y0 + 1;
    ACTIVE_WINDOW.current_x = parent.size.x0 + 1;
    bind('j', select_down);
    bind('k', select_up);
    bind(10, execute); // run 'execute' on enter (10) press
}


void add_entry(MODULE_OPTIONS* opt, void func(void), char caller, const char* text)
{
    opt->options = realloc(opt->options, opt->actions_n + 1);
    opt->options[opt->actions_n].caller = caller;
    opt->options[opt->actions_n].action = func;
    strncpy(opt->options[opt->actions_n].text_msg, text, MAX_LINE_LEN);
    opt->actions_n++;
}
