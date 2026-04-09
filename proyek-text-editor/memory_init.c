#include "editor_memory.h"

void create_editor(Editor *ed) {
  for (int i = 0; i < MAX_ROWS; i++) {
    ed->lines[i][0] = '\0';
    ed->line_lengths[i] = 0;
  }
  ed->line_count = 1;
  ed->cursor_row = 0;
  ed->cursor_col = 0;

  // Inisialisasi Kamera di sudut 0,0
  ed->row_offset = 0;

  ed->is_modified = 0;
  ed->filename[0] = '\0';
}

int is_buffer_full(Editor *ed) {
  // Karena layar sudah dibatasi oleh VISIBLE_ROWS, memori murni bisa dipakai
  // sampai 1000
  return (ed->line_count >= MAX_ROWS);
}

void clear_last_remaining_line(Editor *ed) {
  ed->lines[0][0] = '\0';
  ed->line_lengths[0] = 0;
  ed->cursor_col = 0;
  ed->cursor_row = 0;
}