#include "editor_controller.h"
#include <conio.h>
#include <windows.h>
#include <stdio.h>

void handle_input(Editor *ed) {
    int ch;
    while(1) {
        display_text(ed); 
        ch = _getch();    
        
        if (ch == 0 || ch == 224) { 
            ch = _getch(); 
            switch(ch) {
                case 72: move_cursor_up(ed); break;
                case 80: move_cursor_down(ed); break;
                case 75: move_cursor_left(ed); break;
                case 77: move_cursor_right(ed); break;
            }
        } 
        //  LOGIKA ESC 
        else if (ch == 27) { 
            if (ed->is_modified) {
                // Bersihkan layar sementara untuk meminta konfirmasi
                printf("\033[2J\033[1;1H");
                printf("\n\033[41;97m PERINGATAN !! \033[0m ");
                printf("\nAda perubahan yang belum di-save!\n");
                printf("\nYakin ingin keluar ke menu utama? (y/n): ");
                fflush(stdout);
                
                int confirm = _getch();
                if (confirm == 'y' || confirm == 'Y') {
                    break; // Keluar dari editor
                }
                // Jika loop berlanjut dan display_text akan menggambar ulang editor
            } else {
                break; // Jika tidak ada modifikasi, langsung keluar
            }
        } 
        // ------------------------------
        else if (ch == 19) { save_to_file(ed); } // Ctrl+S
        else if (ch == 4)  { // Ctrl+D untuk Delete Line
            delete_current_line(ed); 
            ed->cursor_col = 0; 
            ed->is_modified = 1; 
        } 
        else if (ch == 8)  { delete_char(ed); } 
        else if (ch == 13) { insert_line(ed); } 
        else if (ch >= 32 && ch <= 126) { insert_char(ed, (char)ch); }
    }
}

void run_main_menu(Editor *ed) {
    setup_console(); 

    int choice;
    do {
        // Bersihkan layar
        printf("\033[2J\033[1;1H"); 
        
        // 1. (Kotak Header)
        printf("\n\n\n"); // Turunkan sedikit dari atap layar
        // Background Cyan Terang (106), Teks Hitam (30)
        printf("\t\033[106;30m                                           \033[0m\n");
        printf("\t\033[106;30m        === Text Editor Bocchi ===         \033[0m\n");
        printf("\t\033[106;30m                                           \033[0m\n\n");

        // 2. DAFTAR MENU 
        printf("\t\033[90m Silakan pilih menu navigasi:\033[0m\n\n"); // Teks instruksi Abu-abu

        // abu gelap (100)
        printf("\t  \033[100;97m 1 \033[0m  Buat File Baru\n\n");
        printf("\t  \033[100;97m 2 \033[0m  Buka File\n\n");
        printf("\t  \033[100;97m 3 \033[0m  Hapus File Fisik\n\n");
        printf("\t  \033[100;97m 4 \033[0m  Keluar dari Program\n\n");

        // AREA INPUT PROMPT
        // Tanda panah Cyan (36)
        printf("\t\033[36m > Pilihan Anda: \033[0m");

        // teks menu ke layar 
        fflush(stdout); 

        choice = _getch();

        if (choice == '1') {
            destroy_editor(ed); 
            printf("\033[?1049h");  
            handle_input(ed);   
            printf("\033[?1049l");  
        }
        else if (choice == '2') {
            char fname[260];
            
            printf("\033[2J\033[1;1H\n\n\t\033[36m > Masukkan nama file:\033[0m ");
            fflush(stdout); 
            fgets(fname, 260, stdin);
            fname[strcspn(fname, "\n")] = 0;
            
            destroy_editor(ed);
            load_from_file(ed, fname);
            
            printf("\033[?1049h");  
            handle_input(ed);
            printf("\033[?1049l");  
        }
        else if (choice == '3') {
            printf("\033[2J\033[1;1H\n\n"); // Bersihkan layar sebelum hapus
            delete_physical_file(ed);
            fflush(stdout); 
            system("pause"); 
        }
        else if (choice == '4') {
            // Peringatan 
            if (ed->is_modified) {
                printf("\n\n\t\033[41;97m PERINGATAN \033[0m Ada file yang belum di-save! Yakin tutup aplikasi? (y/n): ");
                fflush(stdout); 
                if (_getch() != 'y') choice = 0; 
            }
        }
    } while (choice != '4');
}