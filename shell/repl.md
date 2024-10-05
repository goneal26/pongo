# REPL ideas

## Sketches 
prompt example:

```pongo
> :list
 1: short x = 10;
 2: println x;
>
```

## Commands
**No arguments:**
`:list` or `:l` -> print each line of the program
`:run` or `:r` -> run the pongo program
`:quit` or `:q` -> exit the shell
`:<lineNumber>` -> set shell to point at lineNumber, leaves append mode
`:append` or `:a` -> append mode, go to lastLine and increment from there every time a line is entered (on by default, prompt char is "}")

**One argument:**
`:write <filename.pgo>` or `:w <filename.pgo>` -> save program to filename.pgo in current path
`:delete <lineNumber>` or `:d <lineNumber>` -> delete line at lineNumber, shift lines below upwards
`:insert <lineNumber>` or `:i <lineNumber>` -> insert a blank line after lineNumber

