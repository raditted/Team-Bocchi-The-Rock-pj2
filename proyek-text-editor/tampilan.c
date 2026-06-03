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
