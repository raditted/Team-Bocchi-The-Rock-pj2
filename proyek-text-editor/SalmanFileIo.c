#include "SalmanFileIo.h"
#include "RaditBufferBaris.h"
#include "SalmanBufferKarakter.h"

int nama_valid(const char *s) {
    for (int i = 0; s[i]; i++) {
        char c = s[i];
        if (c == '/' || c == '\\' || c == ':' || c == '*' ||
            c == '?' || c == '"' || c == '<' || c == '>' || c == '|')
            return 0;
    }
    return 1;
}

void simpan_file(Editor *ed) {
  if (strlen(ed->nama_file) == 0) {
    do {
      system("cls");
      printf("Masukkan nama file untuk menyimpan: ");
      fgets(ed->nama_file, 260, stdin);
      ed->nama_file[strcspn(ed->nama_file, "\n")] = 0;

      if (!nama_valid(ed->nama_file)) {
        printf("Nama file tidak boleh mengandung: / \\ : * ? \" < > |\n");
        ed->nama_file[0] = '\0'; // reset biar loop ulang
        system("pause");
      }
    } while (!nama_valid(ed->nama_file) || strlen(ed->nama_file) == 0);
  }

  FILE *fp = fopen(ed->nama_file, "w");
  if (!fp)
    return;

  // Tulis ke file per LineNode, lalu per CharNode
  LineNode *baris = ed->head;
  while (baris != NULL) {
    CharNode *c = baris->head_char;
    while (c != NULL) {
      fputc(c->data, fp);
      c = c->next;
    }
    fputc('\n', fp); // Berikan newline sebagai batas antar LineNode
    baris = baris->next;
  }

  fclose(fp);
  ed->sudah_diubah = 0;
}

void buka_file(Editor *ed, const char *nama) {
  FILE *fp = fopen(nama, "r");
  if (!fp)
    return;

  // Bersihkan isi editor saat ini
  reset_editor(ed);
  strcpy(ed->nama_file, nama);

  int ch;
  // Baca karakter satu demi satu dari file
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      // Jika bertemu karakter Enter/Newline, buat LineNode baru
      sisip_baris_baru(ed);
    } else {
      // Jika huruf biasa, simpan sebagai CharNode
      sisip_karakter(ed, (char)ch);
    }
  }

  // Kembalikan kursor ke baris paling atas
  ed->cursor_line = ed->head;
  ed->cursor_char = NULL;
  ed->cursor_row_idx = 0;
  ed->cursor_col_idx = 0;

  fclose(fp);
  ed->sudah_diubah = 0;
}

void hapus_file_fisik(Editor *ed) {
  char target[260];
  system("cls");
  printf("\nMasukkan nama file yang ingin dihapus dari disk: ");
  fgets(target, 260, stdin);
  target[strcspn(target, "\n")] = 0;

  if (remove(target) == 0) {
    printf("\nFile '%s' berhasil dihapus.\n", target);
    // Jika kebetulan file yang dihapus sedang dibuka di memori, reset namanya
    if (strcmp(ed->nama_file, target) == 0) {
      ed->nama_file[0] = '\0';
      ed->sudah_diubah = 1;
    }
  } else {
    printf("\nGagal menghapus file '%s' (mungkin tidak ditemukan).\n", target);
  }
}