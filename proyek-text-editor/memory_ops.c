#include "editor_memory.h"

void shift_chars_right(Editor *ed) {
  int r = ed->cursor_row;
  int c = ed->cursor_col;
  for (int i = ed->line_lengths[r]; i >= c; i--) {
    ed->lines[r][i + 1] = ed->lines[r][i];
  }
}

void insert_char(Editor *ed, char c) {
  int r = ed->cursor_row;
  if (ed->line_lengths[r] + 1 >= ed->line_capacities[r]) {
    ed->line_capacities[r] *= 2;
    ed->lines[r] =
        (char *)realloc(ed->lines[r], ed->line_capacities[r] * sizeof(char));
  }
  shift_chars_right(ed);
  ed->lines[r][ed->cursor_col] = c;
  ed->line_lengths[r]++;
  ed->cursor_col++;
  ed->is_modified = 1;
}