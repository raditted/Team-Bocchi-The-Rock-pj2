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

// Memindahkan sisa karakter dari titik potong ke baris baru (dipakai saat
// tombol Enter)
void pindah_sisa_karakter(LineNode *lama, LineNode *baru,
                          CharNode *titik_potong) {
  if (titik_potong == NULL) {
    // Pindah semuanya (kursor di paling depan)
    baru->head_char = lama->head_char;
    baru->tail_char = lama->tail_char;
    baru->panjang = lama->panjang;

    lama->head_char = NULL;
    lama->tail_char = NULL;
    lama->panjang = 0;
  } else {
    // Pindah sebagian (kursor di tengah)
    baru->head_char = titik_potong->next;
    baru->tail_char = lama->tail_char;

    // Putus koneksi rantai karakter
    if (baru->head_char)
      baru->head_char->prev = NULL;
    titik_potong->next = NULL;
    lama->tail_char = titik_potong;

    // Hitung sisa panjang
    int sisa = 0;
    CharNode *temp = baru->head_char;
    while (temp) {
      sisa++;
      temp = temp->next;
    }

    baru->panjang = sisa;
    lama->panjang -= sisa;
  }
}

void gabung_isi_baris(LineNode *atas, LineNode *bawah) {
  if (bawah->panjang == 0)
    return;

  if (atas->panjang == 0) {
    // Jika baris atas kosong, isi langsung diambil alih
    atas->head_char = bawah->head_char;
    atas->tail_char = bawah->tail_char;
  } else {
    // Jika baris atas ada isinya, sambungkan ekor atas dengan kepala bawah
    atas->tail_char->next = bawah->head_char;
    bawah->head_char->prev = atas->tail_char;
    atas->tail_char = bawah->tail_char;
  }

  atas->panjang += bawah->panjang;
  // Putus ikatan agar memori huruf tidak ikut terhapus saat node bawah dihapus
  bawah->head_char = NULL;
}
