#ifndef EDITOR_NAVIGATION_H
#define EDITOR_NAVIGATION_H

#include "editor_memory.h"

// Membuang/mereset state editor kembali ke kondisi awal (kosong)
void destroy_editor(Editor *ed);

// Untuk Navigasi Cursor
void move_cursor_up(Editor *ed);    // Menggerakkan kursor satu baris ke atas
void move_cursor_down(Editor *ed);  // bawah
void move_cursor_left(Editor *ed);  // kiri
void move_cursor_right(Editor *ed); // kanan

// Untuk Hapus & Manipulasi Teks
void delete_char(Editor *ed);          // hapus karakter di sebelah kiri kursor (tombol Backspace)
void shift_chars_left(Editor *ed);     // geser sisa karakter ke kiri setelah satu karakter dihapus
void merge_with_prev_line(Editor *ed); // gabung baris saat ini ke akhir baris sebelumnya (jika backspace di awal baris)

#endif