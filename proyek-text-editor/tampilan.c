#include "tampilan.h"
#include <windows.h>

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

int baris_konten_maks() { return TINGGI_LAYAR - BARIS_HEADER - BARIS_FOOTER; }

// Hitung berapa baris visual yang dibutuhkan 1 baris logika (soft-wrap)
// Contoh: 200 karakter di layar 80 kolom = 3 baris visual
int baris_visual(LineNode *baris) {
  if (baris->panjang == 0)
    return 1;
  return ((baris->panjang - 1) / LEBAR_LAYAR) + 1;
}

// Tulis spasi sampai kolom ke-80 (agar teks lama tertimpa)
void isi_spasi(char *buf, int dari_kolom) {
  int i;
  for (i = dari_kolom; i < LEBAR_LAYAR; i++) {
    strcat(buf, " ");
  }
}

// Tulis 1 baris teks + padding spasi + newline ke buffer
void tulis_baris(char *buf, const char *teks) {
  strcat(buf, teks);
  isi_spasi(buf, strlen(teks));
  strcat(buf, "\n");
}

// ============================================================
// SCROLL - Pastikan kursor selalu terlihat di layar
// ============================================================

void atur_scroll(Editor *ed) {
  int maks = baris_konten_maks();

  // Jika kursor di atas area tampilan, ikuti kursor ke atas
  if (ed->cursor_row_idx < ed->scroll_offset) {
    ed->scroll_offset = ed->cursor_row_idx;
  }

  // Baris visual kursor di dalam barisnya (efek soft-wrap)
  int visual_kursor = ed->cursor_col_idx / LEBAR_LAYAR;

  // Geser scroll ke bawah sampai kursor terlihat di layar
  while (ed->scroll_offset <= ed->cursor_row_idx) {
    // Hitung total baris visual dari scroll_offset sampai kursor
    int total = 0;
    LineNode *tmp = ed->head;
    int i = 0;

    // Lompat ke scroll_offset
    while (tmp != NULL && i < ed->scroll_offset) {
      tmp = tmp->next;
      i++;
    }

    // Hitung baris visual sampai baris kursor (eksklusif)
    while (tmp != NULL && i < ed->cursor_row_idx) {
      total += baris_visual(tmp);
      tmp = tmp->next;
      i++;
    }

    total += visual_kursor + 1; // Tambah posisi kursor sendiri

    if (total <= maks)
      break;             // Sudah muat!
    ed->scroll_offset++; // Belum muat, geser ke bawah
  }

  // Jika scroll terlalu jauh, kembalikan
  if (ed->scroll_offset > ed->cursor_row_idx) {
    ed->scroll_offset = ed->cursor_row_idx;
  }

  // Tarik ke atas jika ada ruang kosong di bawah
  int total_baris = 0;
  LineNode *tmp = ed->head;
  while (tmp != NULL) {
    total_baris++;
    tmp = tmp->next;
  }

  if (ed->scroll_offset > 0 && total_baris - ed->scroll_offset < maks) {
    ed->scroll_offset = total_baris - maks;
    if (ed->scroll_offset < 0)
      ed->scroll_offset = 0;
  }
}

void tampilkan_editor(Editor *ed) {
  // 1. Anti-flicker: geser kursor ke awal tanpa hapus layar
  kursor_ke_awal();

  // 2. Atur scroll agar kursor terlihat
  atur_scroll(ed);

  char buffer[8192] = "";
  int baris_tercetak = 0;
  int maks = baris_konten_maks();

  // --- HEADER ---
  tulis_baris(buffer, "============================================");
  tulis_baris(buffer, "         TEXT EDITOR BOCCHI (2D DLL)");
  tulis_baris(buffer, "============================================");

  // --- ISI EDITOR ---
  // Lompat ke baris pertama yang terlihat (scroll_offset)
  LineNode *baris = ed->head;
  int idx = 0;
  while (baris != NULL && idx < ed->scroll_offset) {
    baris = baris->next;
    idx++;
  }

  // Cetak baris-baris yang terlihat
  while (baris != NULL && baris_tercetak < maks) {
    CharNode *c = baris->head_char;
    int kolom = 0;
    char tmp[2] = {0, 0};

    // Cetak setiap karakter di baris ini
    while (c != NULL) {
      // Soft-wrap: turun ke baris visual baru jika melewati lebar layar
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

    // Jika kursor di akhir baris dan melampaui batas kolom,
    // buat 1 baris visual kosong agar kursor punya tempat
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
    isi_spasi(buffer, kolom);
    strcat(buffer, "\n");
    baris_tercetak++;

    baris = baris->next;
  }

  // --- PADDING BARIS KOSONG (tanda ~) ---
  while (baris_tercetak < maks) {
    tulis_baris(buffer, "~");
    baris_tercetak++;
  }

  // --- FOOTER / STATUS BAR ---
  tulis_baris(buffer, "--------------------------------------------");

  char status[512];
  sprintf(status, "File: %s %s  | Baris ke-%d | Kolom ke-%d",
          (strlen(ed->nama_file) > 0) ? ed->nama_file : "Untitled",
          ed->sudah_diubah ? "(*)" : "   ", ed->cursor_row_idx + 1,
          ed->cursor_col_idx + 1);
  tulis_baris(buffer, status);

  tulis_baris(buffer, "ESC=Menu | Ctrl+S=Simpan | Ctrl+D=Hapus Baris");

  // 3. Anti-flicker: cetak seluruh buffer sekaligus
  printf("%s", buffer);

  // 4. Posisikan kursor konsol ke posisi kursor editor
  int layar_baris = BARIS_HEADER;
  LineNode *tmp2 = ed->head;
  int i = 0;

  // Lompat ke scroll_offset
  while (tmp2 != NULL && i < ed->scroll_offset) {
    tmp2 = tmp2->next;
    i++;
  }

  // Hitung baris visual sampai baris kursor
  while (tmp2 != NULL && i < ed->cursor_row_idx) {
    layar_baris += baris_visual(tmp2);
    tmp2 = tmp2->next;
    i++;
  }

  // Posisi visual kursor (efek soft-wrap)
  layar_baris += ed->cursor_col_idx / LEBAR_LAYAR;
  int layar_kolom = ed->cursor_col_idx % LEBAR_LAYAR;

  // Tampilkan kursor jika masih di dalam area layar
  if (layar_baris >= BARIS_HEADER &&
      layar_baris < TINGGI_LAYAR - BARIS_FOOTER && layar_kolom >= 0 &&
      layar_kolom < LEBAR_LAYAR) {
    kursor_ke_posisi(layar_baris, layar_kolom);
    tampilkan_kursor();
  }
}