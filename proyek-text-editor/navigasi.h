#ifndef NAVIGASI_H
#define NAVIGASI_H

#include "editor.h"

// sebuah fungsi pergerakan kursor yang memanfaatkan modul buffer_baris dan buffer_karakter
void kursor_kiri(Editor *ed);
void kursor_kanan(Editor *ed);
void kursor_atas(Editor *ed);
void kursor_bawah(Editor *ed);

#endif
