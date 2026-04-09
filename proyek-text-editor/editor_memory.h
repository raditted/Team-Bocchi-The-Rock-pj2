#ifndef EDITOR_MEMORY_H
#define EDITOR_MEMORY_H
#include <stdio.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000

typedef struct {
  char lines[MAX_ROWS][MAX_COLS];
  int line_lengths[MAX_ROWS];
  int line_count;
  int cursor_row;
  int cursor_col;
  int row_offset;
  int is_modified;
  char filename[260];
} Editor;

void create_editor(Editor *ed);
int is_buffer_full(Editor *ed);
void clear_last_remaining_line(Editor *ed);
void insert_char(Editor *ed, char c);
void shift_chars_right(Editor *ed);
void insert_line(Editor *ed);
void shift_lines_down(Editor *ed);
void delete_current_line(Editor *ed);
void shift_lines_up(Editor *ed, int start_idx);

#endif