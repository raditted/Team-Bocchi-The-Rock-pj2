#ifndef EDITOR_CONTROLLER_H
#define EDITOR_CONTROLLER_H

#include "editor_navigation.h"

// Fungsi Publik Anggota 3 (Total: 8)
void run_main_menu(Editor *ed);
void ask_filename(char *buffer, int max_len);
void handle_input(Editor *ed);
void display_text(Editor *ed);
void reset_cursor_to_top(Editor *ed);
void save_to_file(Editor *ed);
void load_from_file(Editor *ed, const char *filename);
void delete_physical_file(Editor *ed);

#endif