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