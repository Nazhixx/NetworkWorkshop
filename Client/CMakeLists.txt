cmake_minimum_required(VERSION 3.16)
project(Client C)

set(CMAKE_C_STANDARD 11)

add_executable(Client main.c)
target_link_libraries(Client ws2_32)