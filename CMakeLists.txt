cmake_minimum_required(VERSION 3.10)
project(snakeGame)

set(snakeGameSources
        functions.c
        main.c
        )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -lncurses")

include_directories(.)

add_executable(snakeGame
        ${snakeGameSources})
