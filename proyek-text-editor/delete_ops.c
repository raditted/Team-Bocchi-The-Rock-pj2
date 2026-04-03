#include "editor_navigation.h"

void destroy_editor(Editor *ed) {
    for (int i = 0; i < ed->line_count; i++) {
        free(ed->lines[i]);
    }
    free(ed->lines);
    free(ed->line_lengths);
    free(ed->line_capacities);
}