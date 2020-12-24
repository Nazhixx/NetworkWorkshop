cmake_minimum_required(VERSION 3.16)
project(Server C)

set(CMAKE_C_STANDARD 11)

add_executable(Server main.c)
target_link_libraries(Server ws2_32)