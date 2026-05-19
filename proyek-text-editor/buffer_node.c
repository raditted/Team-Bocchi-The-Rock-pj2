#include "buffer_node.h"

// Membuat node karakter tunggal
CharNode *buat_char_node(char c) {
  CharNode *baru = (CharNode *)malloc(sizeof(CharNode));
  baru->data = c;
  baru->prev = NULL;
  baru->next = NULL;
  return baru;
}

// Membuat node baris kosong
LineNode *buat_line_node() {
  LineNode *baru = (LineNode *)malloc(sizeof(LineNode));
  baru->head_char = NULL;
  baru->tail_char = NULL;
  baru->panjang = 0;
  baru->prev = NULL;
  baru->next = NULL;
  return baru;
}

// Menghapus semua karakter di satu baris
void free_char_nodes(CharNode *head) {
  CharNode *skrg = head;
  while (skrg != NULL) {
    CharNode *hapus = skrg;
    skrg = skrg->next;
    free(hapus);
  }
}

// Menghapus seluruh baris beserta karakternya
void free_line_nodes(LineNode *head) {
  LineNode *skrg = head;
  while (skrg != NULL) {
    LineNode *hapus = skrg;
    free_char_nodes(hapus->head_char); // Hapus karakter di dalamnya dulu
    skrg = skrg->next;
    free(hapus);
  }
}

// Menyambungkan baris baru ke dalam rantai Doubly Linked List Baris
void sisip_node_baris(Editor *ed, LineNode *lama, LineNode *baru) {
  baru->prev = lama;
  baru->next = lama->next;

  if (lama->next != NULL) {
    lama->next->prev = baru;
  } else {
    ed->tail = baru;
  }
  lama->next = baru;
}

// Melepas baris dari rantai Doubly Linked List Baris (tanpa menghapus memori)
void lepas_node_baris(Editor *ed, LineNode *hapus) {
  if (hapus->prev != NULL) {
    hapus->prev->next = hapus->next;
  } else {
    ed->head = hapus->next;
  }

  if (hapus->next != NULL) {
    hapus->next->prev = hapus->prev;
  } else {
    ed->tail = hapus->prev;
  }
}
