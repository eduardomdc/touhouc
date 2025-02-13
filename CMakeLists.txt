cmake_minimum_required(VERSION 3.10)

project(TouhouC C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

set(MY_SOURCES
    src/main.c
    src/touhou.c
)

add_executable(touhouc ${MY_SOURCES})

target_link_libraries(touhouc raylib)