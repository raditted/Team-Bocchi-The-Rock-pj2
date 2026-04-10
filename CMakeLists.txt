cmake_minimum_required(VERSION 4.2)
project(Team_Bocchi_The_Rock_pj2 C)

set(CMAKE_C_STANDARD 11)

include_directories(proyek-text-editor)

add_executable(Team_Bocchi_The_Rock_pj2
        proyek-text-editor/controller.c
        proyek-text-editor/delete_ops.c
        proyek-text-editor/editor_controller.h
        proyek-text-editor/editor_memory.h
        proyek-text-editor/editor_navigation.h
        proyek-text-editor/file_io.c
        proyek-text-editor/main.c
        proyek-text-editor/memory_init.c
        proyek-text-editor/memory_ops.c
        proyek-text-editor/navigation.c
        proyek-text-editor/ui_display.c)
