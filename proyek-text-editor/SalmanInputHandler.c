#include "SalmanInputHandler.h"
#include "RaditBufferBaris.h"
#include "SalmanBufferKarakter.h"
#include "SalmanFileIo.h"
#include "RaditNavigasi.h"
#include "HaikalTampilan.h"

void handle_input(Editor *ed) {
  int ch;
  while (1) {
    // Tampilkan layar di setiap putaran
    tampilkan_editor(ed);

    // Baca input dari keyboard tanpa Enter (bawaan conio.h)
    ch = _getch();

    // Jika tombol Arrow dipencet (Kodenya 0 atau 224, diikuti kode spesifik)
    if (ch == 0 || ch == 224) {
      ch = _getch();
      switch (ch) {
      case 72:
        kursor_atas(ed);
        break;
      case 80:
        kursor_bawah(ed);
        break;
      case 75:
        kursor_kiri(ed);
        break;
      case 77:
        kursor_kanan(ed);
        break;
      }
    } else if (ch == 27) { // Tombol ESC
      if (ed->sudah_diubah) {
        system("cls");
        printf("\n============================================\n");
        printf("  ADA PERUBAHAN BELUM DISIMPAN!\n");
        printf("  Yakin ingin keluar? (y/n): ");
        int conf = _getch();
        if (conf == 'y' || conf == 'Y')
          break; // Keluar dari loop editor
      } else {
        break;
      }
    } else if (ch == 19) { // Ctrl+S
      simpan_file(ed);
    } else if (ch == 4) { // Ctrl+D
      hapus_baris_penuh(ed);
    } else if (ch == 8) { // Tombol Backspace
      hapus_karakter(ed);
    } else if (ch == 13) { // Tombol Enter
      sisip_baris_baru(ed);
    } else if (ch >= 32 && ch <= 126) { // Hanya karakter yang bisa dicetak
      sisip_karakter(ed, (char)ch);
    }
  }
}

void run_main_menu(Editor *ed) {
  int choice;
  do {
    system("cls");
    printf("\n============================================\n");
    printf("         TEXT EDITOR BOCCHI (DLL)\n");
    printf("============================================\n\n");
    printf("  [1] Buat File Baru\n");
    printf("  [2] Buka File\n");
    printf("  [3] Hapus File Fisik\n");
    printf("  [4] Keluar\n\n");
    printf("  Pilihan Anda: ");

    choice = _getch();

    if (choice == '1') {
      reset_editor(ed);
      handle_input(ed);
    } } else if (choice == '2') {
      char fname[260];
      system("cls");
      printf("\nMasukkan nama file yang ingin dibuka: ");
      fgets(fname, 260, stdin);
      fname[strcspn(fname, "\n")] = 0;

      FILE *cek = fopen(fname, "r");
      if (cek == NULL) {
        printf("File '%s' tidak ditemukan!\n", fname);
        system("pause");
      } else {
        fclose(cek);
        buka_file(ed, fname);
        handle_input(ed);
      }
    }
    else if (choice == '3') {
      hapus_file_fisik(ed);
      system("pause");
    } else if (choice == '4') {
      if (ed->sudah_diubah) {
        printf(
            "\n\nADA PERUBAHAN BELUM DISIMPAN! Yakin tutup aplikasi? (y/n): ");
        if (_getch() != 'y')
          choice = 0;
      }
    }
  } while (choice != '4');
}
