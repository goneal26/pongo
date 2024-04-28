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
        operations.c
        operations.h
        interpreter.c
        interpreter.h
        common.h
        scanner.c
        scanner.h)
