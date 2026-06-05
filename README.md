<table>
  <tr>
    <td width="40%" valign="top">
      <img src="https://github.com/user-attachments/assets/ca7865df-a037-4ea9-b506-7d639df78387" alt="Bocchi Banner" width="100%" style="border-radius: 8px;">
    </td>
    <td width="60%" valign="top">
      <h1>🎸 Bocchi The Rock! ✨ <br> Team 1A-D3</h1>
      <p>
        <em>"We're not just a team, we're a <strong style="color: salmon;">Kessoku Gang!</strong> Get to know the creative minds behind the magic! ✨"</em>
      </p>
      <hr>
      <h3>👥 Anggota Tim ── Proyek 2</h3>
      <p>
        <strong>🎯 Radithya Mahesa Syabil</strong><br>
        <code>251511025</code> |
        🔗 <a href="https://github.com/raditted">@raditted</a>
      </p>
      <p>
        <strong>🧩 Salman Alfarisi Firdaus</strong><br>
        <code>251511030</code> | 
        🔗 <a href="https://github.com/salfffariss">@salfffariss</a>
      </p>
      <p>
        <strong>🎨 Muhamad Haikal Dwi Arya</strong><br>
        <code>251511018</code> |
        🔗 <a href="https://github.com/haikaldwiaryaa">@haikaldwiaryaa</a>
      </p>
      <hr>
      <p align="center">Made with 🩷❤️💛💙 by <strong>BTR Team</strong> | 2026</p>
    </td>
  </tr>
</table>

# Text Editor Bocchi

Text Editor Bocchi adalah text editor berbasis **Command-Line Interface (CLI)** yang dibangun menggunakan bahasa C murni. Editor ini menggunakan struktur data **2D Doubly Linked List** sebagai representasi internal dokumen, di mana seluruh alokasi memori dilakukan secara **dinamis** menggunakan `malloc()` dan `free()` tanpa array statis.

Setiap karakter yang diketik pengguna disimpan sebagai node individual di memori heap. Pendekatan ini memberikan fleksibilitas penuh karena ukuran dokumen tidak dibatasi oleh alokasi awal, melainkan tumbuh dan menyusut sesuai kebutuhan secara real-time.

---

## Fungsi dan Fitur

- Penyisipan dan penghapusan karakter secara real-time
- Navigasi kursor empat arah dengan dukungan soft-wrap
- Manajemen baris: sisip baris baru (Enter), gabung baris (Backspace), hapus baris penuh (Ctrl+D)
- Penyimpanan dan pembukaan file dari disk (Ctrl+S)
- Penghapusan file fisik dari disk
- Soft-wrap otomatis untuk baris yang melebihi lebar layar
- Scrolling vertikal otomatis mengikuti posisi kursor
- Rendering anti-flicker menggunakan buffer dan Win32 Console API
- Konfirmasi keluar jika terdapat perubahan yang belum disimpan
- Validasi nama file terhadap karakter ilegal

---

## Pembagian Tugas

### Haikal

| Modul       | File                        |
| ----------- | --------------------------- |
| Buffer Node | `HaikalBufferNode.c` / `.h` |
| Tampilan    | `HaikalTampilan.c` / `.h`   |

Mengelola pembuatan dan penghapusan node dasar (`CharNode`, `LineNode`), operasi penyambungan dan pelepasan node baris pada rantai DLL, serta seluruh sistem rendering layar yang mencakup header, konten editor dengan soft-wrap, footer status bar, mekanisme scrolling vertikal, dan penempatan kursor konsol menggunakan Win32 API.

### Radit

| Modul        | File                        |
| ------------ | --------------------------- |
| Buffer Baris | `RaditBufferBaris.c` / `.h` |
| Navigasi     | `RaditNavigasi.c` / `.h`    |

Mengelola inisialisasi dan reset editor, operasi tingkat baris seperti penyisipan baris baru saat Enter, penggabungan baris saat Backspace di awal baris, penghapusan baris penuh dengan Ctrl+D, serta seluruh logika navigasi kursor empat arah yang mendukung perpindahan antar baris visual dalam satu baris logika (soft-wrap aware).

### Salman

| Modul           | File                            |
| --------------- | ------------------------------- |
| Buffer Karakter | `SalmanBufferKarakter.c` / `.h` |
| File I/O        | `SalmanFileIo.c` / `.h`         |
| Input Handler   | `SalmanInputHandler.c` / `.h`   |

Mengelola penyisipan dan penghapusan karakter individual pada posisi kursor, pemindahan serta penggabungan rentetan karakter antar baris, operasi file berupa simpan/buka/hapus file dari disk dengan validasi nama, serta penanganan seluruh input keyboard termasuk routing ke modul yang sesuai dan tampilan menu utama.

---

## Visualisasi Struktur Node

### Struktur 2D Doubly Linked List

Dokumen direpresentasikan sebagai dua lapis Doubly Linked List yang saling terhubung.
Lapis pertama adalah rantai **LineNode** (vertikal) yang merepresentasikan baris-baris dokumen.
Setiap LineNode memiliki rantai **CharNode** (horizontal) yang merepresentasikan karakter-karakter di dalam baris tersebut.

```

Editor
|
v
head ──────────────────────────────────────────────────── tail
| |
v v

LineNode 0 <─────prev/next─────> LineNode 1 <───prev/next───> LineNode 2
| | |
| head_char tail_char | head_char tail_char | head_char tail_char
v v v v v v
CharNode<->CharNode<->CharNode CharNode<->CharNode CharNode<->CharNode
[ H ] [ e ] [ l ] [ W ] [ o ] [ ! ] [ ? ]

```

### Detail Node

```

CharNode LineNode
+--------+ +-------------+
| data | (1 karakter) | head_char | --> CharNode pertama
| *prev | <-- node kiri | tail_char | --> CharNode terakhir
| *next | --> node kanan | panjang | jumlah karakter
+--------+ | *prev | <-- baris atas
| *next | --> baris bawah
+-------------+

```

### Struct Editor

```

Editor
+----------------+
| *head | --> LineNode pertama dokumen
| *tail | --> LineNode terakhir dokumen
| *cursor_line | --> LineNode tempat kursor berada
| *cursor_char | --> CharNode di belakang kursor (NULL jika di awal baris)
| cursor_row_idx | indeks baris logika (0-indexed)
| cursor_col_idx | indeks kolom logika (0-indexed)
| scroll_offset | baris pertama yang ditampilkan di layar
| sudah_diubah | flag modifikasi (0 atau 1)
| nama_file[25] | nama file yang sedang dibuka
+----------------+

```

---

## Tabel Fitur dan Fungsi

### Manajemen Node Dasar (`HaikalBufferNode`)

| Fungsi                             | Penjelasan                                                                                               |
| ---------------------------------- | -------------------------------------------------------------------------------------------------------- |
| `buat_char_node(c)`                | Mengalokasikan satu CharNode baru di heap dengan karakter `c`, pointer prev dan next diinisialisasi NULL |
| `buat_line_node()`                 | Mengalokasikan satu LineNode kosong di heap, head/tail char diinisialisasi NULL, panjang 0               |
| `free_char_nodes(head)`            | Membebaskan seluruh rantai CharNode dari head sampai akhir secara iteratif                               |
| `free_line_nodes(head)`            | Membebaskan seluruh rantai LineNode beserta CharNode di dalamnya                                         |
| `sisip_node_baris(ed, lama, baru)` | Menyisipkan LineNode baru tepat setelah LineNode lama dalam rantai DLL baris                             |
| `lepas_node_baris(ed, hapus)`      | Melepaskan LineNode dari rantai DLL tanpa membebaskan memorinya                                          |

### Tampilan dan Rendering (`HaikalTampilan`)

| Fungsi                              | Penjelasan                                                                                                            |
| ----------------------------------- | --------------------------------------------------------------------------------------------------------------------- |
| `kursor_ke_awal()`                  | Memindahkan kursor konsol ke koordinat (0,0) tanpa menghapus layar sebagai teknik anti-flicker                        |
| `kursor_ke_posisi(baris, kolom)`    | Memindahkan kursor konsol ke posisi tertentu menggunakan Win32 SetConsoleCursorPosition                               |
| `tampilkan_kursor()`                | Mengaktifkan kursor kedip bawaan konsol Windows                                                                       |
| `baris_konten_maks()`               | Menghitung jumlah baris visual yang tersedia untuk konten (tinggi layar dikurangi header dan footer)                  |
| `baris_visual(baris)`               | Menghitung berapa baris visual yang dibutuhkan satu baris logika berdasarkan soft-wrap                                |
| `lompat_ke_baris(head, n)`          | Melakukan traversal linked list untuk mendapatkan pointer ke LineNode ke-n                                            |
| `hitung_visual(head, dari, sampai)` | Menghitung total baris visual dari baris logika ke-`dari` hingga ke-`sampai` (eksklusif)                              |
| `isi_spasi(buf, dari_kolom)`        | Menambahkan spasi padding dari kolom tertentu hingga lebar layar ke buffer string                                     |
| `tulis_baris(buf, teks)`            | Menulis teks, padding spasi, dan newline ke buffer sebagai satu baris penuh                                           |
| `atur_scroll(ed)`                   | Mengatur scroll_offset agar kursor selalu terlihat di area konten, mendukung scroll ke bawah dan tarik ke atas        |
| `tampilkan_editor(ed)`              | Fungsi utama rendering: menggambar header, konten dengan soft-wrap, baris kosong (~), footer, dan memposisikan kursor |

### Manajemen Baris (`RaditBufferBaris`)

| Fungsi                       | Penjelasan                                                                                                         |
| ---------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| `buat_editor(ed)`            | Menginisialisasi struct Editor dengan satu LineNode kosong, semua indeks di-reset ke 0                             |
| `reset_editor(ed)`           | Membebaskan seluruh isi editor dan menginisialisasi ulang dari awal                                                |
| `sisip_baris_baru(ed)`       | Memecah baris saat ini di posisi kursor, memindahkan sisa karakter ke baris baru (operasi Enter)                   |
| `hapus_dan_gabung_baris(ed)` | Menggabungkan baris saat ini dengan baris di atasnya, menyambung rantai karakter (operasi Backspace di awal baris) |
| `hapus_baris_penuh(ed)`      | Menghapus seluruh baris saat ini beserta isinya, memindahkan kursor ke baris terdekat (operasi Ctrl+D)             |

### Navigasi Kursor (`RaditNavigasi`)

| Fungsi             | Penjelasan                                                                                                 |
| ------------------ | ---------------------------------------------------------------------------------------------------------- |
| `kursor_kiri(ed)`  | Memindahkan kursor satu posisi ke kiri; jika di awal baris, pindah ke akhir baris sebelumnya               |
| `kursor_kanan(ed)` | Memindahkan kursor satu posisi ke kanan; jika di akhir baris, pindah ke awal baris berikutnya              |
| `kursor_atas(ed)`  | Memindahkan kursor ke atas; mendukung perpindahan antar baris visual dalam satu baris logika yang di-wrap  |
| `kursor_bawah(ed)` | Memindahkan kursor ke bawah; mendukung perpindahan antar baris visual dalam satu baris logika yang di-wrap |

### Manajemen Karakter (`SalmanBufferKarakter`)

| Fungsi                                           | Penjelasan                                                                                                |
| ------------------------------------------------ | --------------------------------------------------------------------------------------------------------- |
| `sisip_karakter(ed, c)`                          | Menyisipkan CharNode baru setelah posisi kursor, memperbarui rantai DLL karakter dan panjang baris        |
| `hapus_karakter(ed)`                             | Menghapus CharNode di posisi kursor (Backspace); jika di awal baris, delegasi ke `hapus_dan_gabung_baris` |
| `pindah_sisa_karakter(lama, baru, titik_potong)` | Memindahkan karakter setelah titik potong dari baris lama ke baris baru, memutus dan menyambung rantai    |
| `gabung_isi_baris(atas, bawah)`                  | Menggabungkan seluruh rantai karakter baris bawah ke ekor baris atas                                      |

### Operasi File (`SalmanFileIo`)

| Fungsi                 | Penjelasan                                                                                        |
| ---------------------- | ------------------------------------------------------------------------------------------------- |
| `nama_valid(s)`        | Memvalidasi nama file terhadap karakter ilegal (/ \\ : \* ? " < > \|)                             |
| `simpan_file(ed)`      | Menyimpan dokumen ke file dengan menulis setiap CharNode per baris, meminta nama jika belum ada   |
| `buka_file(ed, nama)`  | Membuka file dan membangun ulang struktur DLL dengan membaca karakter satu per satu               |
| `hapus_file_fisik(ed)` | Menghapus file dari disk menggunakan `remove()`, reset nama jika file yang dihapus sedang terbuka |

### Penanganan Input (`SalmanInputHandler`)

| Fungsi              | Penjelasan                                                                                                 |
| ------------------- | ---------------------------------------------------------------------------------------------------------- |
| `handle_input(ed)`  | Loop utama editor: membaca input keyboard dan mendelegasikan ke fungsi yang sesuai berdasarkan kode tombol |
| `run_main_menu(ed)` | Menampilkan menu utama dengan opsi buat baru, buka, hapus file, dan keluar                                 |

---

## Alur Kerja Program

### Startup

```

Program dimulai (main.c)
|
v
buat_editor() --> Alokasi 1 LineNode kosong di heap
| Inisialisasi semua pointer dan indeks ke nilai awal
v
run_main_menu() --> Tampilkan menu utama

```

### Menu Utama

```

run_main_menu()
|
+--- [1] Buat File Baru
| |
| v
| reset_editor() --> Bebaskan memori lama, buat ulang dari awal
| |
| v
| handle_input() --> Masuk ke loop editor
|
+--- [2] Buka File
| |
| v
| Validasi file ada --> Jika tidak ada, tampilkan pesan error
| |
| v
| buka_file() --> Reset editor, baca file karakter per karakter
| | Setiap karakter biasa: sisip_karakter()
| | Setiap newline: sisip_baris_baru()
| v
| handle_input() --> Masuk ke loop editor
|
+--- [3] Hapus File Fisik
| |
| v
| hapus_file_fisik() --> remove() dari disk
|
+--- [4] Keluar
|
v
Konfirmasi jika ada perubahan belum disimpan
|
v
free_line_nodes() --> Bebaskan seluruh memori DLL
|
v
Program selesai (return 0)

```

### Loop Editor (handle_input)

```

handle_input()
|
v
+---> tampilkan_editor() --> Render layar
| |
| +-- atur_scroll() : Hitung scroll_offset
| +-- Cetak header : Judul editor
| +-- Cetak konten : Baris-baris dengan soft-wrap
| +-- Cetak footer : Status bar dan shortcut
| +-- Posisikan kursor : Hitung posisi visual dari posisi logika
| |
| v
| \_getch() --> Baca 1 tombol dari keyboard
| |
| +--- Karakter cetak (32-126) --> sisip_karakter()
| +--- Enter (13) --> sisip_baris_baru()
| +--- Backspace (8) --> hapus_karakter()
| +--- Arrow keys (0/224 + kode) --> kursor_atas/bawah/kiri/kanan()
| +--- Ctrl+S (19) --> simpan_file()
| +--- Ctrl+D (4) --> hapus_baris_penuh()
| +--- ESC (27) --> Konfirmasi keluar
| |
+--------<+ (kembali ke render)

```

### Proses Penyisipan Karakter

```

sisip_karakter('A')
|
v
buat_char_node('A') --> malloc(sizeof(CharNode))
| Inisialisasi data='A', prev=NULL, next=NULL
v
Cek posisi kursor:
|
+--- cursor_char == NULL (awal baris)
| |
| v
| Sisip di depan head_char
| baru->next = head_char
| head_char->prev = baru
| head_char = baru
|
+--- cursor_char != NULL (tengah/akhir)
|
v
Sisip setelah cursor_char
baru->prev = cursor_char
baru->next = cursor_char->next
cursor_char->next = baru
|
v
cursor_char = baru (kursor maju)
cursor_col_idx++ (kolom bertambah)
baris->panjang++ (panjang bertambah)
sudah_diubah = 1 (tandai modifikasi)

```

### Proses Scroll

```

atur_scroll()
|
v
Kursor di atas layar? --> scroll_offset = cursor_row_idx
|
v
Hitung baris visual dari scroll_offset sampai kursor
|
+--- Total <= maks (19)? --> Sudah muat, selesai
|
+--- Total > maks? --> scroll_offset++ (geser ke bawah)
| Ulangi perhitungan
v
Cek ruang kosong di bawah
|
+--- Hitung visual dari scroll_offset sampai akhir dokumen
|
+--- Jika ada ruang, coba tarik scroll ke atas
| Cek apakah baris di atas scroll_offset muat ditambahkan
| Jika muat: scroll_offset--, tambah visual
| Jika tidak: berhenti
v
Selesai

```

---

## Cara Menjalankan

### Windows (MinGW GCC)

```

gcc -o bocchi main.c HaikalBufferNode.c HaikalTampilan.c RaditBufferBaris.c RaditNavigasi.c SalmanBufferKarakter.c SalmanFileIo.c SalmanInputHandler.c -Wall

```

```

.\bocchi.exe

```

### Linux (GCC)

Program ini menggunakan `<windows.h>` dan `<conio.h>` yang merupakan API khusus Windows. Untuk menjalankan di Linux, diperlukan modifikasi pada dua bagian:

1. Ganti fungsi `_getch()` dengan pembacaan terminal menggunakan `termios.h`
2. Ganti fungsi Win32 Console API (`SetConsoleCursorPosition`, `SetConsoleCursorInfo`) dengan ANSI escape sequences

Contoh penggantian untuk `kursor_ke_posisi`:

```c
// Windows (asli)
void kursor_ke_posisi(int baris, int kolom) {
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;
  pos.X = kolom;
  pos.Y = baris;
  SetConsoleCursorPosition(h, pos);
}

// Linux (modifikasi)
void kursor_ke_posisi(int baris, int kolom) {
  printf("\033[%d;%dH", baris + 1, kolom + 1);
}
```

Setelah modifikasi, compile dengan:

```
gcc -o bocchi main.c HaikalBufferNode.c HaikalTampilan.c RaditBufferBaris.c RaditNavigasi.c SalmanBufferKarakter.c SalmanFileIo.c SalmanInputHandler.c -Wall
```

```
./bocchi
```

---

## Lessons Learned

### Alokasi Memori Dinamis

Penggunaan `malloc()` dan `free()` secara intensif mengajarkan bahwa setiap alokasi harus memiliki pasangan dealokasi yang jelas. Kebocoran memori menjadi risiko nyata ketika node tidak dibebaskan dengan benar, terutama saat operasi penggabungan dan penghapusan baris di mana kepemilikan pointer berpindah antar node. Memutus koneksi pointer sebelum membebaskan memori menjadi pola penting untuk mencegah dangling pointer dan double free.

### Doubly Linked List sebagai Struktur Data Dokumen

Menggunakan DLL dua dimensi memberikan pemahaman mendalam tentang trade-off antara fleksibilitas dan kompleksitas. Penyisipan dan penghapusan di posisi manapun berjalan dalam O(1) setelah pointer posisi diketahui, tetapi traversal ke posisi tertentu membutuhkan O(n). Setiap operasi manipulasi rantai harus memperhatikan empat pointer sekaligus (prev dan next dari dua node), dan kelalaian pada satu pointer saja dapat merusak seluruh struktur data.

### Soft-Wrap dan Perbedaan Baris Logika vs Visual

Implementasi soft-wrap memperkenalkan konsep bahwa satu baris logika dalam data bisa merepresentasikan lebih dari satu baris visual di layar. Seluruh subsistem yang berinteraksi dengan tampilan — scrolling, navigasi kursor, dan rendering — harus konsisten dalam membedakan kedua konsep ini. Bug yang paling sulit ditemukan berasal dari pencampuran hitungan baris logika dan baris visual dalam satu perhitungan.

### Anti-Flicker Rendering

Mengganti `system("cls")` dengan teknik reposisi kursor ke (0,0) dan overwrite menggunakan buffer string menghilangkan efek kedipan layar. Pendekatan ini mengajarkan bahwa menulis seluruh frame ke satu buffer lalu mencetak sekaligus jauh lebih efisien daripada memanggil `printf()` berkali-kali secara terpisah.

### Modularitas dan Low Coupling

Memecah kode ke dalam modul-modul terpisah berdasarkan tanggung jawab (node, baris, karakter, tampilan, navigasi, file I/O, input) memungkinkan pengerjaan paralel oleh tiga anggota tim. Setiap modul berkomunikasi melalui interface yang didefinisikan di header file, sehingga perubahan internal pada satu modul tidak berdampak pada modul lain selama interface tetap konsisten.

### Debugging Struktur Data Berantai

Debugging linked list tanpa visualisator membutuhkan disiplin mental untuk melacak setiap pointer secara manual. Kesalahan yang paling sering terjadi adalah lupa memperbarui pointer `prev` saat operasi penyisipan, atau lupa memperbarui `tail` saat operasi di akhir rantai. Pola yang efektif adalah selalu menggambar diagram pointer sebelum dan sesudah setiap operasi modifikasi.
