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
    
    // Kosongkan memori lama
    for (int i = 1; i < ed->line_count; i++) free(ed->lines[i]);
    ed->line_count = 1;
    clear_last_remaining_line(ed);
    strcpy(ed->filename, filename);
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0; // Hapus newline
        for(int i = 0; buffer[i] != '\0'; i++) {
            insert_char(ed, buffer[i]);
        }
        insert_line(ed);
    }
    // Hapus baris kosong ekstra di akhir
    if (ed->line_lengths[ed->cursor_row] == 0 && ed->line_count > 1) {
        delete_current_line(ed);
        ed->cursor_row--;
    }