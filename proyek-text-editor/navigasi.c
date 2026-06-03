#include "navigasi.h"

void kursor_kiri(Editor *ed) {
  if (ed->cursor_char != NULL) {
    // Pindah satu karakter ke kiri
    ed->cursor_char = ed->cursor_char->prev;
    ed->cursor_col_idx--;
  } else if (ed->cursor_line->prev != NULL) {
    // Pindah ke akhir baris sebelumnya
    ed->cursor_line = ed->cursor_line->prev;
    ed->cursor_char = ed->cursor_line->tail_char;
    ed->cursor_row_idx--;
    ed->cursor_col_idx = ed->cursor_line->panjang;
  }
}