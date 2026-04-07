#include "editor_navigation.h"

void move_cursor_up(Editor *ed) {
    if (ed->cursor_row > 0) {
        ed->cursor_row--;
        if (ed->cursor_col > ed->line_lengths[ed->cursor_row])
            ed->cursor_col = ed->line_lengths[ed->cursor_row];
    }
}

void move_cursor_down(Editor *ed) {
    if (ed->cursor_row < ed->line_count - 1) {
        ed->cursor_row++;
        if (ed->cursor_col > ed->line_lengths[ed->cursor_row])
            ed->cursor_col = ed->line_lengths[ed->cursor_row];
    }
}

void move_cursor_left(Editor *ed) {
    if (ed->cursor_col > 0) {
        ed->cursor_col--;
    } else if (ed->cursor_row > 0) {
        ed->cursor_row--;
        ed->cursor_col = ed->line_lengths[ed->cursor_row];
    }
}

void move_cursor_right(Editor *ed) {
    if (ed->cursor_col < ed->line_lengths[ed->cursor_row]) {
        ed->cursor_col++;
    } else if (ed->cursor_row < ed->line_count - 1) {
        ed->cursor_row++;
        ed->cursor_col = 0;
    }
}