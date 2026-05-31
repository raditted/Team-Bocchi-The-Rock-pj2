#include "buffer_karakter.h"
#include "buffer_baris.h"
#include "buffer_node.h"

// Menyisipkan karakter di posisi kursor
void sisip_karakter(Editor *ed, char c) {
  CharNode *baru = buat_char_node(c);
  LineNode *baris = ed->cursor_line;

  if (ed->cursor_char == NULL) {
    // Sisip di paling awal baris
    baru->next = baris->head_char;
    if (baris->head_char != NULL) {
      baris->head_char->prev = baru;
    } else {
      baris->tail_char = baru; // Jika baris tadinya kosong
    }
    baris->head_char = baru;
  } else {
    // Sisip setelah cursor_char
    baru->prev = ed->cursor_char;
    baru->next = ed->cursor_char->next;

    if (baru->next != NULL) {
      baru->next->prev = baru;
    } else {
      baris->tail_char = baru; // Jika sisip di paling akhir
    }
    ed->cursor_char->next = baru;
  }

  // Kursor maju ke karakter yang baru disisipkan
  ed->cursor_char = baru;
  ed->cursor_col_idx++;
  baris->panjang++;
  ed->sudah_diubah = 1;
}

// Menghapus karakter (Backspace)
void hapus_karakter(Editor *ed) {
  if (ed->cursor_char == NULL) {
    // Kursor di paling awal baris, hapus/gabung baris dengan baris atas
    if (ed->cursor_line->prev != NULL) {
      hapus_dan_gabung_baris(ed);
    }
    return;
  }

  LineNode *baris = ed->cursor_line;
  CharNode *hapus = ed->cursor_char;

  // Kursor mundur satu langkah sebelum dihapus
  ed->cursor_char = hapus->prev;

  // Putus rantai dari node yang dihapus
  if (hapus->prev != NULL) {
    hapus->prev->next = hapus->next;
  } else {
    baris->head_char = hapus->next;
  }

  if (hapus->next != NULL) {
    hapus->next->prev = hapus->prev;
  } else {
    baris->tail_char = hapus->prev;
  }

  free(hapus);
  ed->cursor_col_idx--;
  baris->panjang--;
  ed->sudah_diubah = 1;
}
