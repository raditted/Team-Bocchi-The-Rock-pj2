#include "buffer_baris.h"
#include "buffer_karakter.h"
#include "buffer_node.h"

// Memulai editor baru dengan 1 baris kosong
void buat_editor(Editor *ed) {
    ed->head = buat_line_node();
    ed->tail = ed->head;
    ed->cursor_line = ed->head;
    ed->cursor_char = NULL;
    ed->cursor_row_idx = 0;
    ed->cursor_col_idx = 0;
    ed->sudah_diubah = 0;
    ed->nama_file[0] = '\0';
}

void reset_editor(Editor *ed) {
    if (ed->head != NULL) {
        free_line_nodes(ed->head);
    }
    buat_editor(ed);
}

// Tombol Enter ini pecah baris saat ini
void sisip_baris_baru(Editor *ed) {
    LineNode *baris_lama = ed->cursor_line;
    LineNode *baris_baru = buat_line_node();

    pindah_sisa_karakter(baris_lama, baris_baru, ed->cursor_char);

    sisip_node_baris(ed, baris_lama, baris_baru);

    // Logik pindahkan kursor
    ed->cursor_line = baris_baru;
    ed->cursor_char = NULL;
    ed->cursor_row_idx++;
    ed->cursor_col_idx = 0;
    ed->sudah_diubah = 1;
}

// tombol Backspace di awal baris: gabung dengan baris atasnya
void hapus_dan_gabung_baris(Editor *ed) {
  if (ed->cursor_line->prev == NULL)
    return; // berarti baris pertama tidak bisa digabung atas

  LineNode *baris_bawah = ed->cursor_line;
  LineNode *baris_atas = baris_bawah->prev;

  // Simpan posisi kolom terakhir untuk kursor
  int kolom_lama = baris_atas->panjang;
  CharNode *kursor_baru = baris_atas->tail_char;

  //  gabungkan rentetan karakter (Low Coupling: Panggil modul karakter)
  gabung_isi_baris(baris_atas, baris_bawah);

  // ini akan putus ikatan wadah baris bawah dari DLL (low coupling, Panggil modul
  // node)
  lepas_node_baris(ed, baris_bawah);

  // bebaskan memori wadah baris (isinya aman karena pointer sudah diputus di
  // gabung_isi_baris)
  free(baris_bawah);

  // 3. Kembalikan kursor ke sambungan
  ed->cursor_line = baris_atas;
  ed->cursor_char = kursor_baru;
  ed->cursor_row_idx--;
  ed->cursor_col_idx = kolom_lama;
  ed->sudah_diubah = 1;
}

void hapus_baris_penuh(Editor *ed) {
  LineNode *hapus = ed->cursor_line;

  // Abaikan jika hanya 1 baris kosong yang tersisa di editor
  if (ed->head == ed->tail && hapus->panjang == 0)
    return;

  // Jika hanya sisa 1 baris (tapi ada isinya), kosongkan isinya saja
  if (ed->head == ed->tail) {
    free_char_nodes(hapus->head_char);
    hapus->head_char = NULL;
    hapus->tail_char = NULL;
    hapus->panjang = 0;
    ed->cursor_char = NULL;
    ed->cursor_col_idx = 0;
    ed->sudah_diubah = 1;
    return;
  }

  // lepas wadah baris dari DLL (low Coupling, panggil modul node)
  lepas_node_baris(ed, hapus);

  // kedua, pindahkan kursor ke baris terdekat (bawah jika ada, atas kalo mentok)
  if (hapus->next != NULL) {
    ed->cursor_line = hapus->next;
  } else {
    ed->cursor_line = hapus->prev;
    ed->cursor_row_idx--;
  }
  ed->cursor_char = NULL;
  ed->cursor_col_idx = 0;

  // ketiga, hancurkan memori baris beserta isinya
  hapus->next = NULL;
  free_line_nodes(hapus);
  ed->sudah_diubah = 1;
}