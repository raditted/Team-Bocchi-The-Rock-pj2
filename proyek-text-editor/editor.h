#ifndef EDITOR_H
#define EDITOR_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAKS_NAMA_FILE 25
#define TINGGI_LAYAR 25
#define LEBAR_LAYAR 60
#define BARIS_HEADER 3 // Jumlah baris untuk header (judul editor)
#define BARIS_FOOTER 3 // Jumlah baris untuk footer (status bar)

// Node untuk menyimpan 1 karakter
typedef struct CharNode {
  char data;
  struct CharNode *prev;
  struct CharNode *next;
} CharNode;

// Node untuk menyimpan 1 baris
typedef struct LineNode {
  CharNode *head_char;   // Karakter pertama
  CharNode *tail_char;   // Karakter terakhir
  int panjang;           // Jumlah karakter di baris ini
  struct LineNode *prev; // Baris sebelumnya (Atas)
  struct LineNode *next; // Baris berikutnya (Bawah)
} LineNode;

// Struct Utama Editor
typedef struct {
  LineNode *head; // Baris pertama editor
  LineNode *tail; // Baris terakhir editor
  // Pointer navigasi langsung
  LineNode *cursor_line; // Baris tempat kursor berada
  CharNode *cursor_char; // Karakter tepat di BELAKANG kursor
  // Informasi untuk UI
  int cursor_row_idx; // Baris ke-berapa secara logika (0-indexed)
  int cursor_col_idx; // Kolom ke-berapa secara logika (0-indexed)
  int scroll_offset;  // Baris pertama yang ditampilkan di layar (0-indexed)
  int sudah_diubah;   // Flag modifikasi
  char nama_file[MAKS_NAMA_FILE];
} Editor;

#endif
