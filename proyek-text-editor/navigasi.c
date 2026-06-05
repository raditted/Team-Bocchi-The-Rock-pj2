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

void kursor_atas(Editor *ed) {
  // Hitung posisi visual kursor saat ini
  int visual_col = ed->cursor_col_idx % LEBAR_LAYAR; // Kolom di baris visual
  int visual_row = ed->cursor_col_idx / LEBAR_LAYAR; // Baris visual ke berapa

  if (visual_row > 0) {
    // KASUS 1: Masih ada baris visual di ATAS dalam baris logika yang SAMA
    // Naik 1 baris visual (mundur 80 kolom)
    int target = (visual_row - 1) * LEBAR_LAYAR + visual_col;

    CharNode *temp = ed->cursor_line->head_char;
    int i = 0;
    ed->cursor_char = NULL;
    while (temp != NULL && i < target) {
      ed->cursor_char = temp;
      temp = temp->next;
      i++;
    }
    ed->cursor_col_idx = i;

  } else if (ed->cursor_line->prev != NULL) {
    // KASUS 2: Sudah di baris visual paling atas lalu pindah ke baris logika atas
    ed->cursor_line = ed->cursor_line->prev;
    ed->cursor_row_idx--;

    // Hitung baris visual terakhir di baris tujuan
    int pjg = ed->cursor_line->panjang;
    int last_vrow = (pjg > 0) ? (pjg - 1) / LEBAR_LAYAR : 0;

    // Target: baris visual terakhir + kolom visual saat ini
    int target = last_vrow * LEBAR_LAYAR + visual_col;
    if (target > pjg)
      target = pjg; // Snap ke akhir jika melebihi panjang baris

    CharNode *temp = ed->cursor_line->head_char;
    int i = 0;
    ed->cursor_char = NULL;
    while (temp != NULL && i < target) {
      ed->cursor_char = temp;
      temp = temp->next;
      i++;
    }
    ed->cursor_col_idx = i;
  }
}