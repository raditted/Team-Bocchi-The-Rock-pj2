#include "editor_controller.h"
#include <conio.h>
#include <windows.h>

void run_main_menu(Editor *ed) {
    int choice;
    do {
        system("cls");
        printf("=== NANO-LITE EDITOR ===\n");
        printf("1. Buat File Baru\n");
        printf("2. Buka File\n");
        printf("3. Hapus File Fisik\n");
        printf("4. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &choice);
    }while (choice != 4);

void ask_filename(char *buffer, int max_len) {
    system("cls");
    printf("Masukkan nama file: ");
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void handle_input(Editor *ed) {
    int ch;
    while(1) {
        display_text(ed); // Panggil UI baru
        ch = _getch();
        
        // Menangkap special keys)
        if (ch == 0 || ch == 224) { 
            ch = _getch();
            switch(ch) {
                case 72: move_cursor_up(ed); break;
                case 80: move_cursor_down(ed); break;
                case 75: move_cursor_left(ed); break;
                case 77: move_cursor_right(ed); break;
            }
        } 
        else if (ch == 27) { // ESCAPE untuk kembali ke menu
            break;
        } 
        else if (ch == 19) { // Ctrl+S
            save_to_file(ed);
        } 
        else if (ch == 8) { // Backspace
            delete_char(ed);
        } 
        else if (ch == 13) { // Enter
            insert_line(ed);
        } 
        else if (ch >= 32 && ch <= 126) { // Karakter yang bisa dicetak
            insert_char(ed, (char)ch);
        }
    }
}