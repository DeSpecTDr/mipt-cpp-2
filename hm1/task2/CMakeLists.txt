cmake_minimum_required(VERSION 3.10)
project(ASCII-Tetris C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Tetris
    ASCII-Tetris/main.c
    ASCII-Tetris/tetris.c
)

target_include_directories(Tetris PRIVATE ASCII-Tetris)
