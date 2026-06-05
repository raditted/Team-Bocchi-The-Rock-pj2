#ifndef BUFFER_NODE_H
#define BUFFER_NODE_H

#include "editor.h"

// Fungsi manajemen memori dasar (Factory Node)
CharNode *buat_char_node(char c);
LineNode *buat_line_node();
void free_char_nodes(CharNode *head);
void free_line_nodes(LineNode *head);

// Fungsi manipulasi penyambungan rantai Node Baris
void sisip_node_baris(Editor *ed, LineNode *lama, LineNode *baru);
void lepas_node_baris(Editor *ed, LineNode *hapus);

#endif
