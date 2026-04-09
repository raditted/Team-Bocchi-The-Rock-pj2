#ifndef EDITOR_NAVIGATION_H
#define EDITOR_NAVIGATION_H

#include "editor_memory.h"

void destroy_editor(Editor *ed);
void move_cursor_up(Editor *ed);
void move_cursor_down(Editor *ed);
void move_cursor_left(Editor *ed);
void move_cursor_right(Editor *ed);
void delete_char(Editor *ed);
void shift_chars_left(Editor *ed);
void merge_with_prev_line(Editor *ed);

#endif