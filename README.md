# The Pongo Programming Language
Pongo is a simple, high-level, garbage-collected, dynamically-typed scripting language with similar structures, syntax, and control flow to languages like BASIC, C, Lox, and Rust. It is an interpreted language with its primary interpreter written in C.

Pongo is both named after the dog from the movie *One Hundred and One Dalmatians* as well as being a joke taken from the game "ping-pong," with the control flow of the language bouncing all over the place like a ping pong ball in a game of ping pong.

I originally started working on this language so that its interpreter could serve as my final project for my *Intro to Programming in C* course, with the goal of having the language's basic control flow, variables, functions, etc. implemented by the end of the project (with implementing Pongo's arrays as a stretch goal). These features will be in the Pongo 0.1 release, with arrays and other stretch goals being in the Pongo 0.2 release.

## Data Types

### Primitives
Pongo, because of its emphasis on 16-bit computing, solely uses signed 16-bit int variables, otherwise known as a "short". They are defined similarly to how they are in C:
```
short n = 13;
short x = 44;
short h = 0xFFFF; // hex values can also be passed, with this corresponding to -1
```

Despite these being the only type in the language thus far, they are still defined with the `short` keyword, as I intend to add other types to the language in the future.

## Expressions

### Arithmetic
Pongo uses your beloved operators from C:
```
x + y;
x - y;
x / y;
x % y;
x * y;
-x;
```

### Comparison
Pongo also takes its comparison operators from C:
```
x < y;
x > y;
x <= y;
x >= y;
x == y;
x != y;
```

These boolean operators also return shorts, with 0 or 0x0000 representing 'false' and -1 or 0xffff representing 'true'. You can also simply write 'true' or 'false' and the language's interpreter will automatically change these to their numeric literal values.

### Logical/Bitwise Operators
These operators appear as keywords in order to be more readable, similar to Python and GDScript. These perform bitwise For example:
```
not true;
true and false;
true or false;
```

These operate on all individual bits of each variable (you know, 'bitwise'). 

### Precedence and Grouping
Pongo's rules regarding operator precedence are the same as C's. You can also group statements using parentheses:
```
short average = (min + max) / 2;
```


## Statements
Statements in Pongo must end in `;`. <!-- TODO: maybe needs more info -->

## Variables <!-- TODO: may be redundant section -->
As I said earlier, variables are defined using the assignment operator `=`, like in Python. Variables can be declared without being defined, with primitive variables being initially defined as `0`:
```
short x = 3;
short y; // y = 0;
```
All variables are visible to any lines of code or instructions that are executed after the variable is defined. But, if you want to "remove" or "free" the variable, freeing it from the memory of the program and resetting its visibility to the program with the keyword `smash` (which is, intentionally, a ping pong term).

```
short x = 3;
smash x;
short y = x; // program exits with error- x not defined/does not exist
```

## Control Flow
As is evident with the name, Pongo is all about a rather messy control flow, 'ping-ponging' around your code file. To do this, Pongo uses labels and goto's for its primary control flow.

### Labels
A label is defined in the program using the keyword `lbl`:
```
lbl main;
short x = 3;
short y = 5;
short z = y / x;
```

Personally, I recommend writing your label names in CAPITALS, so that it's clear to the author of the program or any reader that the label is not, in fact, the name of a variable.

### Gotos
The keyword `goto` causes the program to jump to the line of code defined at the label passed to it. This is where the control flow of the language can get messy, with potential to skip entire code blocks. For example:
```
goto main;

lbl main;
short pi = 22 / 7;
goto next;

lbl skipped;
short tau = pi * 2; // this code gets skipped after jumping to next!

lbl next;
short tau = pi / 2;
```

### Conditional Gotos
The closest thing Pongo has to if-statements is the keyword `thengoto`, appearing in the form `[condition] thengoto [label]`. It functions similarly to an operator, where if the condition on the left is true (i.e. returns -1 or 0xFFFF) then the program will jump to the specified label. For example:
```
// program to check if a number is even
short x = input "Enter a positive number: "; // get user input

((x % 2) == 0) thengoto EVEN;
print "Number is odd"; // this line doesn't get jumped over if the number is odd

lbl EVEN; // the following executes if the number is even
print "Number is even.";
// end of file, program stops
```

## Input/Output
For the initial release of Pongo, the language uses simple input and output statements that work with shorts. For example, the input statement works like so:
```
short x = input "Input x: ";
```
The text "Input x: " is displayed in the terminal/standard output, and the user can input a single short literal (either in decimal, hex with a leading `0x`, or the boolean macros `true` and `false` for -1 and 0). When the user presses <kbd>Enter</kbd>,
that input is stored in the variable declared in the program. If the user enters an invalid input (a character, String, or a number too large to be stored in a short), the program will exit and throw an error.


Output works in a very similar way, using the `print` statement as follows:
```
print "Adding two numbers...";

short x = input "First number: ";
short y = input "Second number: ";
short sum = x + y;

print "Result: ";
print sum;
```

The `print` statement takes a single variable and outputs it to the terminal/standard output.

### String Literals


<!--
	ALL KEYWORDS:
	lbl (like labels)
	short
	exit (exit program)
	true (macro for -1)
	false (macro for 0)
	print (print to console, always on new line)
	input
	smash (kill variable, lets you redeclare it later in program, maybe)
	goto
	thengoto ([condition] thengoto [blockname];)
-->