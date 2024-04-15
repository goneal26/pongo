cmake_minimum_required(VERSION 3.27)
project(cpongo C)

set(CMAKE_C_STANDARD 11)

add_executable(cpongo main.c
        errors.c
        errors.h
        linked_list.c
        linked_list.h
        shortmap.c
        shortmap.h
        execute.c
        execute.h
        parser.c
        parser.h
        operations.c
        operations.h)
