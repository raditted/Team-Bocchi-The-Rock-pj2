#include "buffer_baris.h"
#include "buffer_karakter.h"
#include "buffer_node.h"

// Memulai editor baru dengan 1 baris kosong
void buat_editor(Editor *ed) {
    ed->head = buat_line_node();
    ed->tail = ed->head;
    ed->cursor_line = ed->head;
    ed->cursor_char = NULL;
    ed->cursor_row_idx = 0;
    ed->cursor_col_idx = 0;
    ed->sudah_diubah = 0;
    ed->nama_file[0] = '\0';
}

void reset_editor(Editor *ed) {
    if (ed->head != NULL) {
        free_line_nodes(ed->head);
    }
    buat_editor(ed);
}

// Tombol Enter ini pecah baris saat ini
void sisip_baris_baru(Editor *ed) {
    LineNode *baris_lama = ed->cursor_line;
    LineNode *baris_baru = buat_line_node();

    pindah_sisa_karakter(baris_lama, baris_baru, ed->cursor_char);

    sisip_node_baris(ed, baris_lama, baris_baru);

    // Logik pindahkan kursor
    ed->cursor_line = baris_baru;
    ed->cursor_char = NULL;
    ed->cursor_row_idx++;
    ed->cursor_col_idx = 0;
    ed->sudah_diubah = 1;
}
