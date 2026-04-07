#ifndef EDITOR_CONTROLLER_H
#define EDITOR_CONTROLLER_H

#include "editor_navigation.h" // Membawa serta memory.h

void run_main_menu(Editor *ed);

void ask_filename(char *buffer, int max_len);

void save_to_file(Editor *ed);

void load_from_file(Editor *ed, const char *filename);

void handle_input(Editor *ed);

void delete_physical_file(Editor *ed)
#endif