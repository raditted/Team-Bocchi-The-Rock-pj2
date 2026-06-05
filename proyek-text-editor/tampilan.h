#ifndef TAMPILAN_H
#define TAMPILAN_H

#include "editor.h"

void kursor_ke_awal();
void kursor_ke_posisi(int baris, int kolom);
void tampilkan_kursor();
void atur_scroll(Editor *ed);
int baris_konten_maks();
int baris_visual(LineNode *baris);
void isi_spasi(char *buf, int dari_kolom);
void tulis_baris(char *buf, const char *teks);
void tampilkan_editor(Editor *ed);

#endif