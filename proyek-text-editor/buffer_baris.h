#ifndef BUFFER_BARIS_H
#define BUFFER_BARIS_H

#include "editor.h"

// Fungsi manipulasi node baris
void buat_editor(Editor *ed);
void reset_editor(Editor *ed);
void sisip_baris_baru(Editor *ed);
void hapus_dan_gabung_baris(Editor *ed);
void hapus_baris_penuh(Editor *ed);

// Fungsi untuk menyederhanakan logika
void pindah_sisa_karakter(LineNode *lama, LineNode *baru, CharNode *titik_potong);
void sambung_baris_ke_linked_list(Editor *ed, LineNode *lama, LineNode *baru);
void gabung_isi_baris(LineNode *atas, LineNode *bawah);
void lepas_baris_dari_rantai(Editor *ed, LineNode *hapus);

#endif
