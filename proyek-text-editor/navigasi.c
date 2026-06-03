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

void kursor_kanan(Editor *ed) {
  if (ed->cursor_char == NULL && ed->cursor_line->head_char != NULL) {
    // Kursor tadinya di awal, maju 1 ke karakter pertama
    ed->cursor_char = ed->cursor_line->head_char;
    ed->cursor_col_idx++;
  } else if (ed->cursor_char != NULL && ed->cursor_char->next != NULL) {
    // Pindah 1 karakter ke kanan
    ed->cursor_char = ed->cursor_char->next;
    ed->cursor_col_idx++;
  } else if (ed->cursor_line->next != NULL) {
    // Pindah ke awal baris berikutnya
    ed->cursor_line = ed->cursor_line->next;
    ed->cursor_char = NULL;
    ed->cursor_row_idx++;
    ed->cursor_col_idx = 0;
  }
}