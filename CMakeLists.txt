cmake_minimum_required(VERSION 4.0)
project(Trabalho1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Trabalho1 main.c
        ordenacao.h)

include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(Trabalho1 ${CMAKE_SOURCE_DIR}/lib/libordenacao3.a)
