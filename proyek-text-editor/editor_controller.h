#ifndef EDITOR_CONTROLLER_H
#define EDITOR_CONTROLLER_H

#include "editor_navigation.h" // Membawa serta memory.h

void run_main_menu(Editor *ed);

void ask_filename(char *buffer, int max_len);

#endif