#include "RaditBufferBaris.h"
#include "HaikalBufferNode.h"
#include "SalmanInputHandler.h"

// Entry point untuk program
int main() {
  Editor ed;
  buat_editor(&ed);   // Inisialisasi struktur utama
  run_main_menu(&ed); // Masuk ke loop antarmuka

  // Sebelum keluar, pastikan semua memori DLL dikembalikan ke OS
  if (ed.head != NULL) {
    free_line_nodes(ed.head);
  }

  return 0;
}