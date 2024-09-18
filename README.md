# The Pongo Programming Language

**Version b1.0**

Pongo is a simple, 16-bit toy programming language. An initial proof-of-concept was written in pure C, with the latest version being written in Go. The language is inherently non-structured (similar to TinyBASIC), and relies heavily on bitwise operations. 

This language is still being actively worked on- the beta I have released so far is just the minimal working version and more features are on the way (see the *Roadmap* section).

## Installation
Prerequisite: have Go installed (version 1.18 or higher)

 1) Clone this repository
 2) Navigate to this repository's directory and run `go build -o bin/`
 3) A new binary file called "pongo" will be created in the "bin" directory for you to add to your path
 4) Now, run `pongo [filename].pgo` on any Pongo scripts you write to run the interpreter! (`.pgo` is Pongo's file extension)

## Features
 - labels and gotos, with labels also serving as a kind of constant.
 - 16-bit signed integers (a "short")
 - arrays of 16-bit signed integers (called a "buff" in pongo)
 - the `@` operator for checking single boolean flags as well as for buff indexing
 - NO scoping, variables can be freed (or "smashed") at any point in the program's evaluation.

Read SPECIFICATION.md for more information. For syntax highlighting for Sublime Text, check the `misc` directory. For some example pongo scripts, check the `examples` directory.

## Roadmap
 - [x] Operator Precedence Parsing (Go rewrite)
 - [x] List type (`buff`)
 - [x] `sizeof` unary operator
 - [x] Bit-index operator (`@`)
 - [ ] Lists that can contain lists
 - [ ] List resizing
 - [ ] TinyBASIC-style repl
 - [ ] Basic call-stack and returning for `goto`
 - [ ] Proper if-statements (instead of "if x then goto (label)" just "if x then (statement)")
