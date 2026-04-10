#include "editor_memory.h"
#include <string.h>

void shift_chars_right(Editor *ed) {
  int r = ed->cursor_row;
  int c = ed->cursor_col;
  int len = ed->line_lengths[r] - c;
  // Geser blok memori secara instan tanpa looping
  if (len > 0) {
    memmove(&ed->lines[r][c + 1], &ed->lines[r][c], len);
  }
}

void insert_char(Editor *ed, char c) {
  int r = ed->cursor_row;
  // Jika baris sudah menabrak batas kanan layar, tolak input
  if (ed->line_lengths[r] >= MAX_COLS - 1)
    return;

  shift_chars_right(ed);
  ed->lines[r][ed->cursor_col] = c;
  ed->line_lengths[r]++;
  ed->cursor_col++;
  ed->is_modified = 1;
}

void shift_lines_down(Editor *ed) {
  int r = ed->cursor_row;
  int count = ed->line_count - (r + 1);

  if (count > 0) {
    memmove(&ed->lines[r + 2], &ed->lines[r + 1], count * MAX_COLS);
    memmove(&ed->line_lengths[r + 2], &ed->line_lengths[r + 1],
            count * sizeof(int));
  }
}

void shift_lines_up(Editor *ed, int start_idx) {
  int count = ed->line_count - 1 - start_idx;

  if (count > 0) {
    memmove(&ed->lines[start_idx], &ed->lines[start_idx + 1], count * MAX_COLS);
    memmove(&ed->line_lengths[start_idx], &ed->line_lengths[start_idx + 1],
            count * sizeof(int));
  }
}

void insert_line(Editor *ed) {
  // Jika menekan Enter menabrak batas bawah layar, tolak input
  if (is_buffer_full(ed))
    return;

  shift_lines_down(ed);

  int r = ed->cursor_row;
  int new_r = r + 1;

  // Bawa sisa teks ke baris bawahnya
  strcpy(ed->lines[new_r], &ed->lines[r][ed->cursor_col]);
  ed->line_lengths[new_r] = ed->line_lengths[r] - ed->cursor_col;

  // Potong baris atas
  ed->lines[r][ed->cursor_col] = '\0';
  ed->line_lengths[r] = ed->cursor_col;

  ed->line_count++;
  ed->cursor_row++;
  ed->cursor_col = 0;
  ed->is_modified = 1;
}

void delete_current_line(Editor *ed) {
  if (ed->line_count == 1) {
    clear_last_remaining_line(ed);
    return;
  }
  shift_lines_up(ed, ed->cursor_row);
  ed->line_count--;
  // Jika kita menghapus baris paling bawah, tarik kursor naik 1 tingkat
  if (ed->cursor_row >= ed->line_count) {
    ed->cursor_row = ed->line_count - 1;
  }

  // Cegah kursor keluar batas kolom jika ditarik ke baris yang lebih pendek
  if (ed->cursor_col > ed->line_lengths[ed->cursor_row]) {
    ed->cursor_col = ed->line_lengths[ed->cursor_row];
  }
}