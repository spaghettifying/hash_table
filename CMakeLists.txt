cmake_minimum_required(VERSION 3.26)
project(hash_table C)

set(CMAKE_C_STANDARD 11)

add_executable(hash_table src/hash_table.c
        include/hash_table.h
        src/prime.c
        include/prime.h
        tests/hash_table_test.c)
