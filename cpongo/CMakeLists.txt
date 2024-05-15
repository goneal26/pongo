cmake_minimum_required(VERSION 3.27)
project(cpongo C)

set(CMAKE_C_STANDARD 11)

add_executable(cpongo main.c
        messages.c
        messages.h
        expression.c
        expression.h
        common.h
        variable_table.c
        variable_table.h
        interpreter.c
        interpreter.h
)
