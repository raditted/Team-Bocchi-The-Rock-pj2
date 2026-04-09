#include "editor_controller.h"

void save_to_file(Editor *ed) {
    if (strlen(ed->filename) == 0) ask_filename(ed->filename, 260);
    FILE *fp = fopen(ed->filename, "w");
    if (!fp) return;
    for (int i = 0; i < ed->line_count; i++) {
        fprintf(fp, "%s\n", ed->lines[i]);
    }
    fclose(fp);
    ed->is_modified = 0;
}

void load_from_file(Editor *ed, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    
    create_editor(ed);
    strcpy(ed->filename, filename);
    
    char buffer[MAX_COLS * 2];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Load karakter satu per satu 
        for(int i = 0; buffer[i] != '\0'; i++) {
            insert_char(ed, buffer[i]);
        }
        // jika load file sudah mentok
        if (!is_buffer_full(ed)) {
            insert_line(ed);
        }
    }
    
    if (ed->line_lengths[ed->cursor_row] == 0 && ed->line_count > 1) {
        delete_current_line(ed);
        ed->cursor_row--;
    }
    
    reset_cursor_to_top(ed);
    ed->is_modified = 0;
    fclose(fp);
}