#ifndef EDITOR_MEMORY_H
#define EDITOR_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **lines;
  int *line_lengths;
  int *line_capacities;
  int line_count;
  int line_capacity;
  int cursor_row;
  int cursor_col;
  int is_modified;
  char filename[260];
} Editor;

void expand_capacity(Editor *ed);
void create_editor(Editor *ed);

#endif