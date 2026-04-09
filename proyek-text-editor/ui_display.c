#include "editor_controller.h"
#include <stdio.h>
#include <string.h>
#include <windows.h> 

void moveCursor(int row, int col) {
    printf("\033[%d;%dH", row + 2, col + 1); 
}

void display_text(Editor *ed) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    
    int window_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int window_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    int console_width = window_width - 1; 
    if (console_width <= 0) console_width = 79;
    
    int visible_rows = window_height - 4; 
    if (visible_rows <= 0) visible_rows = 20;

    if (ed->cursor_row < ed->row_offset) {
        ed->row_offset = ed->cursor_row;
    }

    int cursor_phys_y;
    do {
        cursor_phys_y = 0;
        for (int i = ed->row_offset; i < ed->cursor_row; i++) {
            cursor_phys_y += (ed->line_lengths[i] / console_width) + 1;
        }
        cursor_phys_y += (ed->cursor_col / console_width);

        if (cursor_phys_y >= visible_rows) {
            ed->row_offset++; 
        }
    } while (cursor_phys_y >= visible_rows);

    while (ed->row_offset > 0) {
        int current_total_phys = 0;
        for (int i = ed->row_offset; i < ed->line_count; i++) {
            current_total_phys += (ed->line_lengths[i] / console_width) + 1;
        }

        int test_offset = ed->row_offset - 1;
        int test_cursor_y = 0;
        for (int i = test_offset; i < ed->cursor_row; i++) {
            test_cursor_y += (ed->line_lengths[i] / console_width) + 1;
        }
        test_cursor_y += (ed->cursor_col / console_width);

        if (current_total_phys < visible_rows && test_cursor_y < visible_rows) {
            ed->row_offset--;
        } else {
            break; 
        }
    }

    printf("\033[?25l"); 
    printf("\033[1;1H"); 

    // 1. Panel Biru Gelap
	printf("\033[90m"); // Foreground Dark Gray (90)
    for(int i = 0; i < console_width; i++) putchar('-'); // Menggunakan strip tipis
    printf("\033[0m\033[K\n");

    int current_phys_y = 0;
    int target_cursor_x = 0;
    int target_cursor_y = 0;

    for (int i = ed->row_offset; i < ed->line_count && current_phys_y < visible_rows; i++) {
        int len = ed->line_lengths[i];
        int chunks = (len / console_width) + 1;
        
        for (int chunk_idx = 0; chunk_idx < chunks && current_phys_y < visible_rows; chunk_idx++) {
            
            if (i == ed->cursor_row && chunk_idx == (ed->cursor_col / console_width)) {
                target_cursor_x = ed->cursor_col % console_width;
                target_cursor_y = current_phys_y;
            }

            int start_idx = chunk_idx * console_width;
            int chars_to_print = len - start_idx;
            if (chars_to_print > console_width) chars_to_print = console_width;
            
            for (int c = 0; c < chars_to_print; c++) {
                putchar(ed->lines[i][start_idx + c]);
            }
            
            printf("\033[K\n"); 
            current_phys_y++;
        }
    }

    while (current_phys_y < visible_rows) {
        printf("\033[K\n");
        current_phys_y++;
    }

    // 2. Biru Gelap

  	printf("\033[90m"); 
    for(int i = 0; i < console_width; i++) putchar('-'); 
    printf("\033[0m\033[K\n");

    // 3. Informasi Dokumen & Lencana

    // Background Putih (47), Teks Hitam (30) untuk nama file
    printf("\033[47;30m  File: %s %s  \033[0m", 
           (strlen(ed->filename) > 0) ? ed->filename : "Untitled",
           ed->is_modified ? "(*)" : "   ");
           
    // Background Cyan Terang (106), Teks Hitam (30)
    printf("\033[106;30m  Baris: %d | Kolom: %d  \033[0m\033[K\n", 
           ed->cursor_row + 1, ed->cursor_col + 1);

  
    // 4. Keycap Shortcuts

    // Background Abu-abu Gelap (100), Teks Putih (97)
    // Teks keterangan menggunakan teks biasa
    printf("  \033[100;97m ESC \033[0m Menu Utama    \033[100;97m Ctrl+S \033[0m Simpan    \033[100;97m Ctrl+D \033[0m Hapus Baris \033[K");

    moveCursor(target_cursor_y, target_cursor_x);
    printf("\033[?25h"); 
    fflush(stdout); 
}