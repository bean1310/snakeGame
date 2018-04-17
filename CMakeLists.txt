cmake_minimum_required(VERSION 3.10)
project(snakeGame C)

set(snakeGameSources
        src/functions.c
        src/main.c
        )

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_C_FLAGS "-Wall")

add_executable(snakeGame
        ${snakeGameSources})

target_link_libraries(snakeGame ncurses)