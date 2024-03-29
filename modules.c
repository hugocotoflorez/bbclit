#include "bbclit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


// have to migrate DIMENSION as ptr to PAIR


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


/*
 * ---------- SELECTION BOX ----------
 */

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


void reset_color()
{
    wprintf(L"\e[0m");
    fflush(OUT_STREAM);
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
    reset_color();
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
    reset_color();
}


void execute()
{
    // BINDED FUNCTION
    // can be use as a trigger
    ACTIVE_WINDOW.trigger(ACTIVE_WINDOW.current_ptr.y0);
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
    reset_color();
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
void selection_box(void trigger(int), BOX parent, MODULE_OPTIONS opt)
{
    ACTIVE_WINDOW.size        = parent.size;
    ACTIVE_WINDOW.settigns    = parent.settings;
    ACTIVE_WINDOW.current_ptr = parent.box_ptr;
    ACTIVE_WINDOW.options     = opt;
    ACTIVE_WINDOW.trigger     = trigger;
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


/*
 * ---------- TEXT BOX ----------
 */

struct __paragraph
{
    int line_length, lines;
    char* text;
    PAIR current_ptr;
    PAIR max_position;
    DIMENSION parent_size;
    int lMARGIN;
    BOX parent;
    struct CUSTOMIZE_SETTINGS settings;
} PARAGRAPH;


void free_paragraph()
{
    free(PARAGRAPH.text);
}

void initialize_paragraph(BOX parent)
{
    PARAGRAPH.line_length  = parent.size.x1 - parent.size.x0;
    PARAGRAPH.lines        = 0;
    PARAGRAPH.text         = NULL;
    PARAGRAPH.current_ptr  = (PAIR){ parent.box_ptr.x0, parent.box_ptr.y0 };
    PARAGRAPH.max_position = (PAIR){ parent.size.x1 - 1, parent.size.y1 - 1 };
    PARAGRAPH.parent_size  = parent.size;
    PARAGRAPH.settings     = parent.settings;
    PARAGRAPH.lMARGIN      = 2;
    PARAGRAPH.parent = parent;
    atexit(free_paragraph);
}


void __LOGPRINT(char* msg, int n)
{
    FILE* f = fopen("log.txt", "a");
    fprintf(f, "[%s] (%d):\t%s\n", __TIME__, n, msg);
    fclose(f);
}


// scroll: at what line to start
// -  0: first line
// - -1: (lines - box\_size) or 0  -- JUST THIS OPTION DONE
// [Scroll text at new entry]
// - >0: start at line 'scroll'
void write_paragraph(int scroll)
{
    clear_box(PARAGRAPH.parent);
    int start_line;
    if(scroll < 0)
    {
        start_line =
        PARAGRAPH.lines - PARAGRAPH.parent_size.y1 + PARAGRAPH.parent_size.y0 + 1;
        PARAGRAPH.current_ptr.y = PARAGRAPH.parent_size.y0 + 1;
        start_line              = (start_line > 0) ? start_line : 0;
        for(int i = start_line; i < PARAGRAPH.lines; i++)
        {
            cursor_goto(PARAGRAPH.parent_size.x0 + PARAGRAPH.lMARGIN,
            PARAGRAPH.current_ptr.y);
            apply_color(PARAGRAPH.settings.color.text);
            PARAGRAPH.current_ptr.y++;
            wprintf(L"%s", PARAGRAPH.text + PARAGRAPH.line_length * i);
            //__LOGPRINT(text, __LINE__);
            reset_color(); // ya tiene fflush por lo que no se lo meto al wprintf
        }
    }
    else
    {
        // TODO
    }
}


// for now text cannot exceed line length
void appendnl_text(char* text)
{
    //__LOGPRINT("-- APPEND NL --", __LINE__);
    if(strlen(text) >= PARAGRAPH.line_length)
    {
        //__LOGPRINT("LINE TOO LONG", __LINE__);
        return; // text is too large
    }
    PARAGRAPH.lines++;
    char* temp =
    realloc(PARAGRAPH.text, sizeof(char) * PARAGRAPH.line_length * PARAGRAPH.lines);
    PARAGRAPH.text = (temp != NULL) ? temp : PARAGRAPH.text;
    if(temp == NULL)
    {
        //__LOGPRINT("Cannot allocate more text", __LINE__ - 3);
        return;
    }
    strcpy(PARAGRAPH.text + (PARAGRAPH.lines - 1) * PARAGRAPH.line_length, text);
    if(PARAGRAPH.current_ptr.y <= PARAGRAPH.max_position.y) // not at last line
    {
        cursor_goto(
        PARAGRAPH.parent_size.x0 + PARAGRAPH.lMARGIN, PARAGRAPH.current_ptr.y);
        apply_color(PARAGRAPH.settings.color.text);
        PARAGRAPH.current_ptr.y++;
        wprintf(L"%s", text);
        //__LOGPRINT(text, __LINE__);
        reset_color(); // ya tiene fflush por lo que no se lo meto al wprintf
    }
    else // cursor at last line
    {
        write_paragraph(-1);
        //__LOGPRINT("AT LAST LINE", __LINE__);
    }
}

/*
 * ---------- INPUT BOX ----------
 */

void get_input(BOX parent, char* buffer, int length, bool* cancellation_signal)
{
    clear_box(parent);
    //fflush(OUT_STREAM);
    //__LOGPRINT("Into get_input", __LINE__);
    cursor_goto(parent.box_ptr.x0, parent.box_ptr.y0);
    *cancellation_signal = true;
    show_cursor();
    //__LOGPRINT("Start input_string", __LINE__);
    input_string(OUT_STREAM, buffer, length, parent.size.x1 - parent.size.x0-2);
    //__LOGPRINT("Out input_string with:", __LINE__);
    //__LOGPRINT(buffer, __LINE__);
    hide_cursor();
    *cancellation_signal = false;
}
