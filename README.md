# The Pongo Programming Language
<!-- note: ctrl-shift-g for preview in sublime -->
Pongo is a simple, high-level, garbage-collected, dynamically-typed scripting language with similar structures, syntax, and control flow to languages like BASIC, C, Lox, and Rust. It is an interpreted language with its primary interpreter written in C.

Pongo is both named after the dog from the movie *One Hundred and One Dalmatians* as well as being a joke taken from "ping-pong" (or table tennis, if you prefer) with the control flow of the language bouncing all over the place like a ping-pong ball. This is reflected in other features of the language, such as using jargon/terminology from table tennis as keywords in the language.

I originally started working on this language so that its interpreter could serve as my final project for my *Intro to Programming in C* course, with the goal of having the language's basic control flow, variables, functions, etc. implemented by the end of the project (with implementing Pongo's arrays as a stretch goal). These features will be in the Pongo 0.1 release, with arrays and other stretch goals being in the Pongo 0.2 release.

Pongo somewhat fits into the C language family when it comes to syntax, mostly due to the use of semicolons to terminate statements and the use of `//` to mark an line comment. The semicolons were mostly added because it made the language much easier for the interpreter parse (as then I could just split-up the text by semicolons to get each instruction), as well as allowing for multiple instructions per line if the user desires.

## Data Types

### Primitives
Pongo, because of its emphasis on 16-bit computing, solely uses signed (2's complement) 16-bit int variables, otherwise known as a "short". They are defined similarly to how they are in C:

```
short n = 13;
short x = 44;
short h = 0xFFFF; // hex values can also be passed, with this corresponding to -1
// when using hex values literals, characters after the x are not case sensitive,
// so 0xffff and 0xFFFF are both fine.
```

As a reminder, the largest value a signed 16-bit number can hold is 32767 in decimal (or 0x7FFF in hex), and the smallest value is -32768 in decimal (or 0x8000 in hex). The language does have a way of handling overflows, described later.

Despite this being the only type in the language thus far, they are still defined with the `short` keyword, as I intend to add other types to the language in the future.

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

**Note on overflows:** Pongo handles overflows by printing a warning that an overflow has occured to the standard output, but does not exit the program. If an overflow occurs, the variable will then hold the number represented by the amount of bits that overflowed. For example, since the language uses 16-bit numbers, an overflow occurring after adding two numbers, say *n* and *m*, and storing the result of that computation in another variable, say *x*, would output the warning message and then *x* would hold the value *(n + m) mod 16*.

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

These boolean operators also return shorts, with 0 or 0x0000 representing 'false' and -1 or 0xFFFF representing 'true'. You can also simply write 'true' or 'false' and the language's interpreter will automatically change these to their numeric literal values. My reasoning for choosing -1 and 0xFFFF for 'true' instead of simply using 1 as C does is simple- the logical operators (explained in the next section) act as bitwise-operators. Applying a bitwise 'and' between a 16-bit number and 'true' should (in my opinion) return the original 16-bit number, and since applying a bitwise 'and' to a 16-bit number and 0xFFFF returns the original 16-bit number, and 0xFFFF in base 2 is the 2's complement representation of -1, it makes sense for 'true' to be a macro for -1 instead of 1 as it is in C.

This feature is subject to change however, as I intend to add more types (such as a proper Boolean type) to the language in the future (see the Future Features section).

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

## Variables 
<!-- TODO: may be redundant section -->
As I said earlier, variables are defined using the assignment operator `=`, like in Python. Variables can be declared without being defined, with primitive variables being initially defined as `0`:
```
short x = 3;
short y; // y = 0;
```
Variable names/identifiers have a max character limit of 32 characters, and have the same naming conventions as in C. Personally, I prefer using `snake_case` for Pongo variables.

All variables are visible to any lines of code or instructions that are executed after the variable is defined. But, if you want to "remove" or "free" the variable, freeing it from the memory of the program and resetting its visibility to the program with the keyword `smash` (which is, intentionally, a ping pong term).

```
short x = 3;
smash x;
short y = x; // program exits with error- x not defined/does not exist
```

Under the hood, variables are stored in a structure similar to a table/dictionary using the identifier (string for the variable name) as a key and the short as a value. The `smash` keyword simply removes this entry from the table. This is important as the max size of the table (i.e. the maximum amount of variables a program can have allocated in memory at a given time) is 65536 (note- a single unsigned short, or two bytes), so using `smash` will free-up room for another variable. When the program exits, all variables are freed.

To free all of the variables in the program at once, use `smashall` (which is a unique statement that takes no arguments).

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

Personally, I recommend writing your label names using what is sometimes known as `SCREAMING_SNAKE_CASE` (i.e. just regular `snake_case` but in all-capital letters), so that it's clear to whoever reads the program that the label is not, in fact, the name of a variable. I would recommend using `camelCase` or `PascalCase`, but I'm contemplating making those case conventions (specifically `PascalCase`) required for user-defined types if/when I implement them in the future.

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
exit; // exit the program here instead of the end of the file

lbl EVEN; // the following executes if the number is even
print "Number is even.";
// end of file, program stops
```

## Input/Output
For the initial release of Pongo, the language uses simple input and output statements that work with shorts. For example, the input statement works like so:
```
short x = input "Input x: ";
```
The text "Input x: " is displayed in the terminal/standard output, and the user can input a single short literal (either in decimal, hex with a leading `0x`, or the boolean macros `true` and `false` for -1 and 0). When the user presses <kbd>Enter</kbd>, that input is stored in the variable declared in the program. If the user enters an invalid input (a character, String, or a number too large to be stored in a short), the program will exit and throw an error.


Output works in a very similar way, using the `print` statement as follows:

```
print "Adding two numbers...";

short x = input "First number: ";
short y = input "Second number: ";
short sum = x + y;

print "Result: ";
print sum;
```

The `print` statement takes a single variable and outputs it to the terminal/standard output. `print` does not print to a new line, instead printing onto the same line- to print something on a separate line, use `println`. Using the `println` statement with no arguments will just print a blank line to the standard output (think running `printf("\n");` in C).

### Formatting
The `input` statement automatically infers the format of the input- it first ensures that the input is, in fact, a short (and will exit the program with an error if this is not the case), then ensures that the input can fit within a short (otherwise exiting the program with an overflow error) and will accept inputs of either decimal integers or hex literals (such as `0xFFFF` or `0xffff`, ignoring case, just as variables do).

There's no real string formatting the `print` or `println` statements as of right now, as there isn't a string type in the language. As I said before, these statements can take at most one argument- either text in double-quotes, or a single variable to output to the standard output. I may extend this later, but for now printing the value of a short variable prints its value as a signed decimal (base-10) integer, with no option to print the value as a hex (base-16) number.

### Note on String Literals
String literals are surrounded by double-quotes, as in many other programming languages. As of right now, these literals are solely used for input/output, but later I do hope to extend the language to allow for a String type (or at least an equivalent to a String type, such as a character array or a byte array).

## Future Features
These are the features of the language that I hope to implement in the future (i.e. as of right now, after I submit Pongo and its C-based interpreter as my final project for my class):

- More robust types (such as byte and bool, and unsigned types)
- Arrays (these would work similarly to C's arrays, having a type and predefined size, but maybe with some built-in methods for resizing, removing, etc.)
- User-defined types (similar to C's use of `typedef` and structs, but I plan on doing this using a custom data structure that would theoretically function as an array that can hold values of any type)
- Generics (for use in creating custom types)
- Proper loops (instead of just using labels)
- Memory management (something akin to pointers in C, maybe a bit safer though)
- File reading/writing
- Bootstrapping the interpreter (i.e. writing the Pongo interpreter in Pongo!)
- Something akin to libraries/packages with a package manager/environment system (kinda like Python's pip and venv system, or Rust's cargo system, although this is a very-far-future goal)

**Side note**: even though having "proper" loops would be convenient for a programmer and much more conventional, I think it undermines the whole point/gimmick of the language (that the control flow *ping-pongs* about), so adding this later isn't a promise and I don't plan to prioritize it over the other features in this list.

### Type Aspirations
Notice how I didn't specify adding a `char` type. This is because despite `char` being treated as a "primitive" type in many languages, different languages do in fact represent their `char` type in different ways (for example, C uses 8-bit ASCII values but Java uses 16-bit unicode values), and I don't want to commit to a specific representation yet, or really, at all. This leads into my motivation for how this language's hypothetical future type system would work.

When it comes to adding new types, I ideally want to have at minimum basic types for signed and unsigned bytes and shorts, and then a robust system of user-defined types, so that other types (such as ASCII characters, Strings, etc) I can simply define in Pongo itself instead of creating data structures for those types within the interpreter in C. Then I could package those types in something akin to a Pongo standard library that a user can import.

Also, notice how my list of future features lacks *functions*. I may add functions in the future, but initially the closest thing I may have to functions would simply be the equivalent of object-methods in other languages, functions intended only for acting on and making modifications to specific data structures. For the time being, using labels should be fine- especially for making simple demonstration programs.

Other types (such as 32 and 64 bit ints, floats, etc) I would also have as their own separate library/package (I could jokingly call it the *modern-computing* package, perhaps), because I want to emphasize that Pongo to me is, at heart, a "toy-language" for my 16-bit computing hobby.

<!--
	ALL KEYWORDS:
	lbl 
	short
	exit (exit program)
	true (macro for -1 or 0xFFFF)
	false (macro for 0 or 0x0000)
	print (print to console)
	println (print to console on a new line)
	input
	smash (kill variable, lets you redeclare it later in program)
	smashall (kill all variables in the program)
	goto
	thengoto ([condition] thengoto [labelname];)
-->