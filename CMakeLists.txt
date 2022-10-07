cmake_minimum_required(VERSION 3.0)
project(tttrsstts)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Curses REQUIRED)
find_package(SDL2 REQUIRED)

add_library(tttrsstts OBJECT src/game.c src/field.c src/updates.c src/inputs.c src/piece.c)
add_executable(main_curses src/main_curses.c)
add_executable(main_sdl src/main_sdl.c)

include_directories(include)
target_include_directories(main_curses PRIVATE ${CURSES_INCLUDE_DIRS})
target_include_directories(main_sdl PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(main_curses tttrsstts)
target_link_libraries(main_sdl tttrsstts)
target_link_libraries(main_curses ${CURSES_LIBRARIES})
target_link_libraries(main_sdl ${SDL2_LIBRARIES})
