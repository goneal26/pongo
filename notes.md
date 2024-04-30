# The Interpreter Process

## First pass- labels
2) Go through each string in the file ending with a `;`, if it starts with the keyword "lbl" then store the index of that instruction in a dictionary (NOTE: syntax `lbl (NAME);`).
	- i.e. starting with counter = 0, check if the string starts with `lbl`. Then store the first word after `lbl`, i.e. `(NAME)` in the dictionary like (counter, `NAME`). When a semicolon `;` is encountered, increment the counter. Continue until the end of the file.
	- The dictionary will consist of the label names (strings) as keys, and the instruction/line number as values.

## Parsing
1) Read file, create array of strings called "instructions".
	- each instruction in instructions is a string for a statement, splitting by semicolon (;)
	- don't forget to ignore leading and trailing whitespace

## Second pass- actually interpreting
3) This is the actual "interpreter" part. Go through each instruction (from top to bottom) and execute them. This will essentially be a switch-statement. If the interpreter encounters either the end of the instructions list (i.e. the end of the file) or the `exit;` instruction, stop the program (and free memory to avoid leaks).

### Instruction types:
Instructions are statements ending in semicolons, which may also contain an expression.

#### Working with variables
A given program can have a maximum of 65536 (2^16) variables at once. If a user tries to create a variable when this maximum has reached, the program will exit with an error (like, "memory capacity reached").

`short (name);` -> create a short with identifier (name) and initialize it to 0.
`short (name) = (expression);` -> create a short with identifier (name) and initialize to the evaluated expression

NOTE: These work by adding these to a variable dictionary. The variable dictionary uses the (name) as an identifier and either 0 or the evaluated (expression) as a value. The dictionary will keep count of how many variables currently exist, so here you'd increment that counter.

`(name) = (expression);` -> assign the value of (expression) to the variable (if it exists) called (name)

NOTE: Set the value of the variable in the dictionary identified by (name) to the value of the (expression), if it exists.

`smash (name);` -> "Free" the variable from memory, essentially destroying it, leaving it inaccessible (as it doesn't exist anymore) to any later instructions in the program.

`smashall;` -> "Free" every currently existing variable from memory.

NOTE: Essentially remove the identifier/value pair from the variable dictionary. The dictionary will keep count of how many variables currently exist, so here you'd decrement that counter. With `smashall`, you clear out the entire dictionary and reset the count to 0.

#### Control flow (working with labels)
`goto (NAME);` -> Skip ahead to the instruction at the index marked by `lbl (NAME);`
`(boolean expression) thengoto (NAME);` -> If (boolean expression) evaluates to `true` (a.k.a. `-1` or hex `0xffff`), skip ahead to the instruction at the index marked by `lbl (NAME);`. If the expression evaluates to `false`(a.k.a. `0` or hex `0x0000`) continue to the next instruction. Otherwise, throw an error (like "statement requires boolean expression") and stop the program.

`exit;` -> Stop running the program, stopping the interpreter with a successful exit code. This will also free all existing variables. This also occurs when the interpreter reaches the end of the file.

NOTE: this is NOT a label, it is its own instruction.

#### Helper statements (input/output, debugging stuff)
`clock;` -> Print the current running time of the program as of this line to the standard output.

`print (expression);` -> Print the evaluation of the expression to the standard output.
`println (expression);` -> Print the evaluation of the expression to the standard output on a separate line.

`print "(string)";` -> Print the string literal (string) to the standard output.
`println "(string)";` -> Print the string literal (string) to the standard output on a separate line.

### Expressions
Expressions use the same operators as C for basic arithmetic, and the same rules for precedence. You can also use parentheses just as in C.

Boolean expressions use boolean comparison operators (`==`, `<`, `>`, `<=`, `>=`) which evaluate to either `true` (`-1` or `0xffff`) or `false` (`0` or `0x0000`)