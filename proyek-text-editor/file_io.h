#ifndef FILE_IO_H
#define FILE_IO_H

#include "editor.h"

// Fungsi operasi file ke disk komputer
void simpan_file(Editor *ed);
void buka_file(Editor *ed, const char *nama);
void hapus_file_fisik(Editor *ed);

#endif
