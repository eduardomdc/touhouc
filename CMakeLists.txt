cmake_minimum_required(VERSION 3.10)

project(TouhouC C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

set(MY_SOURCES
    src/main.c
    src/touhou.c
    src/player.c
    src/bullets.c
    src/timer.c
    src/enemy.c
    src/enemy_factory.c
    src/firing_patterns.c
    src/spawner.c
    src/sprite.c
    src/assets.c
    src/compact_array.c
    src/items.c
)

add_executable(touhouc ${MY_SOURCES})

target_link_libraries(touhouc raylib)