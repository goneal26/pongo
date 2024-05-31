cmake_minimum_required(VERSION 3.22)
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
        token.c
        token.h
)

if (MSVC)
    add_compile_options(/std:c11)  # Ensure C11 standard is used
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # Disable secure CRT warnings
endif()