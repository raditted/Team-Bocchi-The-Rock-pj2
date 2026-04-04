#include "editor_memory.h"

void create_editor(Editor *ed) {
  ed->line_capacity = 10;
  ed->line_count = 1;
  ed->lines = (char **)malloc(ed->line_capacity * sizeof(char *));
  ed->line_lengths = (int *)malloc(ed->line_capacity * sizeof(int));
  ed->line_capacities = (int *)malloc(ed->line_capacity * sizeof(int));

  if (!ed->lines || !ed->line_lengths || !ed->line_capacities)
    exit(1);

  // Inisialisasi baris pertama
  ed->line_capacities[0] = 32;
  ed->lines[0] = (char *)malloc(ed->line_capacities[0] * sizeof(char));
  ed->lines[0][0] = '\0';
  ed->line_lengths[0] = 0;

  ed->cursor_row = 0;
  ed->cursor_col = 0;
  ed->is_modified = 0;
  ed->filename[0] = '\0';
}

void expand_capacity(Editor *ed) {
  ed->line_capacity *= 2;
  ed->lines = (char **)realloc(ed->lines, ed->line_capacity * sizeof(char *));
  ed->line_lengths =
      (int *)realloc(ed->line_lengths, ed->line_capacity * sizeof(int));
  ed->line_capacities =
      (int *)realloc(ed->line_capacities, ed->line_capacity * sizeof(int));
}

void clear_last_remaining_line(Editor *ed) {
  ed->lines[0][0] = '\0';
  ed->line_lengths[0] = 0;
  ed->cursor_col = 0;
  ed->cursor_row = 0;
}