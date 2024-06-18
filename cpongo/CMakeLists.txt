cmake_minimum_required(VERSION 3.22)
project(cpongo C)

set(CMAKE_C_STANDARD 11)

add_executable(cpongo main.c
        common.c
        common.h
        interpreter.c
        interpreter.h
        scanner.c
        scanner.h
        parser.c
        parser.h
)
