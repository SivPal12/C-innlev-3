cmake_minimum_required (VERSION 2.8)
project (innlevering02)
add_definitions(-std=c11)

add_executable(bin.out program.c)

target_link_libraries(bin.out ncurses)
