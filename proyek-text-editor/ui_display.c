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

    
}