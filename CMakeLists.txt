cmake_minimum_required(VERSION 3.29)
project(hash_table_implementation C)

set(CMAKE_C_STANDARD 23)

add_executable(hash_table_implementation main.c
        linked_list.c
        linked_list.h
        hash_table.c
        hash_table.h)
