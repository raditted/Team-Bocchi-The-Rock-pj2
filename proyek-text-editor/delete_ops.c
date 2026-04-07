#include "editor_navigation.h"

void destroy_editor(Editor *ed) {
    for (int i = 0; i < ed->line_count; i++) {
        free(ed->lines[i]);
    }
    free(ed->lines);
    free(ed->line_lengths);
    free(ed->line_capacities);
}

void delete_char(Editor *ed) {
    if (ed->cursor_col > 0) {
        shift_chars_left(ed);
        ed->cursor_col--;
        ed->is_modified = 1;
    } else if (ed->cursor_row > 0) {
        merge_with_prev_line(ed);
    }
}