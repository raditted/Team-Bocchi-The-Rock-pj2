cmake_minimum_required(VERSION 4.2)
project(proyek_text_editor C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(proyek_text_editor
        editor.h
        HaikalBufferNode.c
        HaikalBufferNode.h
        HaikalTampilan.c
        HaikalTampilan.h
        main.c
        RaditBufferBaris.c
        RaditBufferBaris.h
        RaditNavigasi.c
        RaditNavigasi.h
        SalmanBufferKarakter.c
        SalmanBufferKarakter.h
        SalmanFileIo.c
        SalmanFileIo.h
        SalmanInputHandler.c
        SalmanInputHandler.h)