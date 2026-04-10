#include "editor_controller.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>


void setup_console() {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);

  static char render_buffer[65536];
  setvbuf(stdout, render_buffer, _IOFBF, sizeof(render_buffer));
}

void reset_cursor_to_top(Editor *ed) {
  ed->cursor_row = 0;
  ed->cursor_col = 0;
}

void ask_filename(char *buffer, int max_len) {
  printf("\033[2J\033[1;1H");
  printf("\n\n\t\033[36m > Masukkan nama file:\033[0m ");
  fflush(stdout);
  fgets(buffer, max_len, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
}

void handle_input(Editor *ed) {
  int ch;
  while (1) {
    display_text(ed);
    ch = _getch();

    if (ch == 0 || ch == 224) {
      ch = _getch();
      switch (ch) {
      case 72:
        move_cursor_up(ed);
        break;
      case 80:
        move_cursor_down(ed);
        break;
      case 75:
        move_cursor_left(ed);
        break;
      case 77:
        move_cursor_right(ed);
        break;
      }
    } else if (ch == 27) {
      if (ed->is_modified) {
        printf("\033[2J\033[1;1H");
        printf("\n\033[31m[PERINGATAN]\033[0m\n");
        printf("Ada perubahan yang belum di-save!\n");
        printf("Yakin ingin keluar ke menu utama? (y/n): ");
        fflush(stdout);

        int confirm = _getch();
        if (confirm == 'y' || confirm == 'Y')
          break;
      } else {
        break;
      }
    } else if (ch == 19) {
      save_to_file(ed);
    } else if (ch == 4) {
      delete_current_line(ed);
      ed->cursor_col = 0;
      ed->is_modified = 1;
    } else if (ch == 8) {
      delete_char(ed);
    } else if (ch == 13) {
      insert_line(ed);
    } else if (ch >= 32 && ch <= 126) {
      insert_char(ed, (char)ch);
    }
  }
}

void run_main_menu(Editor *ed) {
  setup_console();

  int choice;
  do {
    printf("\033[2J\033[1;1H");

    printf("\n\n\n");
    printf(
        "\t\033[106;30m                                           \033[0m\n");
    printf(
        "\t\033[106;30m        === Text Editor Bocchi ===         \033[0m\n");
    printf(
        "\t\033[106;30m                                           \033[0m\n\n");

    printf("\t\033[90m Silakan pilih menu navigasi:\033[0m\n\n");

    printf("\t  \033[100;97m 1 \033[0m  Buat File Baru\n\n");
    printf("\t  \033[100;97m 2 \033[0m  Buka File\n\n");
    printf("\t  \033[100;97m 3 \033[0m  Hapus File Fisik\n\n");
    printf("\t  \033[100;97m 4 \033[0m  Keluar dari Program\n\n");

    printf("\t\033[36m > Pilihan Anda: \033[0m");
    fflush(stdout);

    choice = _getch();

    if (choice == '1') {
      destroy_editor(ed);
      printf("\033[?1049h");
      handle_input(ed);
      printf("\033[?1049l");
    } else if (choice == '2') {
      char fname[260];
      ask_filename(fname, 260);

      if (strlen(fname) > 0) {
        FILE *file_check = fopen(fname, "r");
        if (file_check == NULL) {
          printf("\033[2J\033[1;1H\n\n");
          printf("\t\033[41;97m ERROR \033[0m File '%s' tidak ditemukan!\n",
                 fname);
          fflush(stdout);
          Sleep(1500);
        } else {
          fclose(file_check);
          destroy_editor(ed);
          load_from_file(ed, fname);
          printf("\033[?1049h");
          handle_input(ed);
          printf("\033[?1049l");
        }
      }
    } else if (choice == '3') {
      printf("\033[2J\033[1;1H\n\n");
      delete_physical_file(ed);
      fflush(stdout);
      system("pause");
    }
  } while (choice != '4');
}