#include "editor_memory.h"

void shift_chars_right(Editor *ed) {
  int r = ed->cursor_row;
  int c = ed->cursor_col;
  for (int i = ed->line_lengths[r]; i >= c; i--) {
    ed->lines[r][i + 1] = ed->lines[r][i];
  }
}