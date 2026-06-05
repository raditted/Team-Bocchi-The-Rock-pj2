#include "HaikalTampilan.h"
#include <windows.h>

// === Fungsi Konsol ===

void kursor_ke_awal() {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos = {0, 0};
  SetConsoleCursorPosition(h, pos);
}

void kursor_ke_posisi(int baris, int kolom) {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;
  pos.X = kolom;
  pos.Y = baris;
  SetConsoleCursorPosition(h, pos);
}

void tampilkan_kursor() {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 25;
  info.bVisible = 1;
  SetConsoleCursorInfo(h, &info);
}

// === Fungsi Pembantu ===

int baris_konten_maks() { return TINGGI_LAYAR - BARIS_HEADER - BARIS_FOOTER; }

int baris_visual(LineNode *baris) {
  if (baris->panjang == 0)
    return 1;
  return ((baris->panjang - 1) / LEBAR_LAYAR) + 1;
}

// Lompat ke baris ke-n di linked list (helper agar tidak berulang)
LineNode *lompat_ke_baris(LineNode *head, int n) {
  LineNode *tmp = head;
  int i = 0;
  while (tmp != NULL && i < n) {
    tmp = tmp->next;
    i++;
  }
  return tmp;
}

// Hitung total baris visual dari baris 'dari' sampai baris 'sampai' (eksklusif)
int hitung_visual(LineNode *head, int dari, int sampai) {
  LineNode *tmp = lompat_ke_baris(head, dari);
  int total = 0;
  int i = dari;
  while (tmp != NULL && i < sampai) {
    total += baris_visual(tmp);
    tmp = tmp->next;
    i++;
  }
  return total;
}

void isi_spasi(char *buf, int dari_kolom) {
  for (int i = dari_kolom; i < LEBAR_LAYAR; i++)
    strcat(buf, " ");
}

void tulis_baris(char *buf, const char *teks) {
  strcat(buf, teks);
  isi_spasi(buf, strlen(teks));
  strcat(buf, "\n");
}

// === Scroll ===

void atur_scroll(Editor *ed) {
  int maks = baris_konten_maks();

  // Kursor di atas layar → ikuti ke atas
  if (ed->cursor_row_idx < ed->scroll_offset)
    ed->scroll_offset = ed->cursor_row_idx;

  int visual_kursor = ed->cursor_col_idx / LEBAR_LAYAR;

  // Geser scroll ke bawah sampai kursor terlihat
  while (ed->scroll_offset <= ed->cursor_row_idx) {
    int total = hitung_visual(ed->head, ed->scroll_offset, ed->cursor_row_idx);
    total += visual_kursor + 1;
    if (total <= maks)
      break;
    ed->scroll_offset++;
  }

  // Jika scroll terlalu jauh, kembalikan
  if (ed->scroll_offset > ed->cursor_row_idx)
    ed->scroll_offset = ed->cursor_row_idx;

  // Tarik ke atas jika ada ruang kosong di bawah
  if (ed->scroll_offset > 0) {
    // Hitung visual dari scroll_offset sampai akhir dokumen
    LineNode *tmp = lompat_ke_baris(ed->head, ed->scroll_offset);
    int total_visual = 0;
    while (tmp != NULL) {
      total_visual += baris_visual(tmp);
      tmp = tmp->next;
    }

    // Coba tarik scroll ke atas selama masih muat
    while (ed->scroll_offset > 0) {
      LineNode *atas = lompat_ke_baris(ed->head, ed->scroll_offset - 1);
      int extra = baris_visual(atas);
      if (total_visual + extra <= maks) {
        ed->scroll_offset--;
        total_visual += extra;
      } else {
        break;
      }
    }
  }
}

// === Render Utama ===

void tampilkan_editor(Editor *ed) {
  kursor_ke_awal();
  atur_scroll(ed);

  char buffer[8192] = "";
  int baris_tercetak = 0;
  int maks = baris_konten_maks();

  // Header
  tulis_baris(buffer,
              "============================================================");
  tulis_baris(buffer, "              TEXT EDITOR BOCCHI (2D DLL)");
  tulis_baris(buffer,
              "============================================================");

  // Isi editor (mulai dari scroll_offset)
  LineNode *baris = lompat_ke_baris(ed->head, ed->scroll_offset);

  while (baris != NULL && baris_tercetak < maks) {
    CharNode *c = baris->head_char;
    int kolom = 0;
    char tmp[2] = {0, 0};

    // Cetak karakter dengan soft-wrap
    while (c != NULL) {
      if (kolom >= LEBAR_LAYAR) {
        strcat(buffer, "\n");
        baris_tercetak++;
        kolom = 0;
        if (baris_tercetak >= maks)
          break;
      }
      tmp[0] = c->data;
      strcat(buffer, tmp);
      kolom++;
      c = c->next;
    }

    // Baris visual ekstra jika kursor di akhir baris melampaui batas kolom
    if (baris == ed->cursor_line && baris->panjang > 0 &&
        ed->cursor_col_idx / LEBAR_LAYAR > (baris->panjang - 1) / LEBAR_LAYAR) {
      if (baris_tercetak < maks) {
        isi_spasi(buffer, kolom);
        strcat(buffer, "\n");
        baris_tercetak++;
        kolom = 0;
      }
    }

    // Padding spasi + newline
    if (baris_tercetak < maks) {
      isi_spasi(buffer, kolom);
      strcat(buffer, "\n");
      baris_tercetak++;
    }
    baris = baris->next;
  }

  // Baris kosong (tanda ~)
  while (baris_tercetak < maks) {
    tulis_baris(buffer, "~");
    baris_tercetak++;
  }

  // Footer
  tulis_baris(buffer,
              "------------------------------------------------------------");
  char status[512];
  sprintf(status, "File: %s %s  | Baris ke-%d | Kolom ke-%d",
          (strlen(ed->nama_file) > 0) ? ed->nama_file : "Untitled",
          ed->sudah_diubah ? "(*)" : "   ", ed->cursor_row_idx + 1,
          ed->cursor_col_idx + 1);
  tulis_baris(buffer, status);
  tulis_baris(buffer, "ESC=Menu | Ctrl+S=Simpan | Ctrl+D=Hapus Baris");

  printf("%s", buffer);

  // Posisikan kursor konsol
  int layar_baris = BARIS_HEADER + hitung_visual(ed->head, ed->scroll_offset,
                                                 ed->cursor_row_idx);
  layar_baris += ed->cursor_col_idx / LEBAR_LAYAR;
  int layar_kolom = ed->cursor_col_idx % LEBAR_LAYAR;

  if (layar_baris >= BARIS_HEADER &&
      layar_baris < TINGGI_LAYAR - BARIS_FOOTER && layar_kolom >= 0 &&
      layar_kolom < LEBAR_LAYAR) {
    kursor_ke_posisi(layar_baris, layar_kolom);
    tampilkan_kursor();
  }
}
