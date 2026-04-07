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

void shift_lines_down(Editor *ed) {
  for (int i = ed->line_count; i > ed->cursor_row + 1; i--) {
    ed->lines[i] = ed->lines[i - 1];
    ed->line_lengths[i] = ed->line_lengths[i - 1];
    ed->line_capacities[i] = ed->line_capacities[i - 1];
  }
}

void shift_lines_up(Editor *ed, int start_idx) {
  for (int i = start_idx; i < ed->line_count - 1; i++) {
    ed->lines[i] = ed->lines[i + 1];
    ed->line_lengths[i] = ed->line_lengths[i + 1];
    ed->line_capacities[i] = ed->line_capacities[i + 1];
  }
}

void insert_line(Editor *ed) {
  if (ed->line_count >= ed->line_capacity)
    expand_capacity(ed);
  shift_lines_down(ed);

  int r = ed->cursor_row;
  int new_r = r + 1;

  int chars_to_move = ed->line_lengths[r] - ed->cursor_col;
  ed->line_capacities[new_r] = (chars_to_move > 32) ? chars_to_move * 2 : 32;
  ed->lines[new_r] = (char *)malloc(ed->line_capacities[new_r] * sizeof(char));

  strcpy(ed->lines[new_r], &ed->lines[r][ed->cursor_col]);
  ed->line_lengths[new_r] = chars_to_move;
  ed->lines[r][ed->cursor_col] = '\0';
  ed->line_lengths[r] = ed->cursor_col;

  ed->line_count++;
  ed->cursor_row++;
  ed->cursor_col = 0;
  ed->is_modified = 1;
}