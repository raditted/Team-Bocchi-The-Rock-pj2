#include "editor_navigation.h" 
 
void destroy_editor(Editor *ed) { 
    create_editor(ed);
} 
 
void shift_chars_left(Editor *ed) { 
    int r = ed->cursor_row; 
    int c = ed->cursor_col; 
    int len = ed->line_lengths[r] - c; 
    if (len > 0) { 
        memmove(&ed->lines[r][c - 1], &ed->lines[r][c], len); 
    } 
    ed->line_lengths[r]--; 
} 
 
void merge_with_prev_line(Editor *ed) { 
    int r = ed->cursor_row; 
    int prev_r = r - 1; 
     
    if (ed->line_lengths[prev_r] + ed->line_lengths[r] >= MAX_COLS - 1) 
return; 
     
    int target_col = ed->line_lengths[prev_r]; 
     
    strcat(ed->lines[prev_r], ed->lines[r]); 
    ed->line_lengths[prev_r] += ed->line_lengths[r]; 
     
    delete_current_line(ed); 
     
    ed->cursor_row = prev_r; 
    ed->cursor_col = target_col;  
    ed->is_modified = 1; 
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
