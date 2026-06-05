#ifndef BUFFER_KARAKTER_H
#define BUFFER_KARAKTER_H

#include "editor.h"

// Fungsi manipulasi karakter di dalam sebuah baris
void sisip_karakter(Editor *ed, char c);
void hapus_karakter(Editor *ed);

// Fungsi pembantu pemindahan rentetan karakter antar baris
void pindah_sisa_karakter(LineNode *lama, LineNode *baru,
                          CharNode *titik_potong);
void gabung_isi_baris(LineNode *atas, LineNode *bawah);

#endif
