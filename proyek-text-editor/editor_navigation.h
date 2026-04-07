#ifndef EDITOR_NAVIGATION_H
#define EDITOR_NAVIGATION_H

#include "editor_memory.h"

void destroy_editor(Editor *ed);
void shift_chars_left(Editor *ed);
void merge_with_prev_line(Editor *ed);

#endif