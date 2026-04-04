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