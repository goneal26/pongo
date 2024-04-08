# The Pongo Programming Language
Pongo is a simple, high-level, garbage-collected, dynamically-typed scripting language with similar structures, syntax, and control flow to languages like BASIC, C, Lox, and Rust. It is an interpreted language with its primary interpreter written in C.

Pongo is both named after the dog from the movie *One Hundred and One Dalmatians* as well as being a joke taken from the game "ping-pong," with the control flow of the language bouncing all over the place like a ping pong ball in a game of ping pong.

I originally started working on this language so that its interpreter could serve as my final project for my *Intro to Programming in C* course, with the goal of having the language's basic control flow, variables, functions, etc. implemented by the end of the project (with implementing Pongo's arrays as a stretch goal). These features will be in the Pongo 0.1 release, with arrays and other stretch goals being in the Pongo 0.2 release.

## Data Types

### Primitives
These are the language's primitive types.
 - **Booleans**, with the literal values `true` and `false`.
 - **Numbers**, 32-bit double precision floating point numbers represented with literals like `1234`, `12.32`, or `-3` (although I may make a 16-bit version of the language that uses signed shorts for its numbers later)
 - **Strings**, encoded the same way as Strings in C as well as using the same escape characters (such as `\n`) (ASCII characters terminated by an implied null character `\0`) represented with literals `"like this"`
 - **Nil**, a no-value/null data type, represented as `nil`

Primitive-type variables are defined using the keyword `let`:
```
let n = 13;
let x = 44.5;
let truth = true;
let empty = nil;
let msg = "Hello, world!\n";
```

<!--
### Non-Primitives
These are the non-primitive types, i.e. the data structures that exist in the language.

#### Arrays
First there are **arrays**, working the same way that arrays in C do, with their size being set initially, only they can hold multiple different types. If an array is defined with a size but not populated in that same line, it is filled with `nil`. Arrays are also defined with the keyword `let`, as well as using brackets (`[` and `]`), for example:
```
let arr[5] = [0, nil, true, "hi", 13.2];
let empty_arr[3];

empty_arr[0] = "first";
empty_arr[1] = 2;
empty_arr[2] = 3.0;

let n = empty_arr.length; // here, n=3
let is_empty = arr.empty // true if array is nil for every element
```
-->

## Expressions

### Arithmetic
Pongo uses your beloved operators from C:
```
x + y;
x - y;
x / y;
x * y;
-x;
```

### tostring
`tostring` is a unary operator that converts any variable that gets passed to it into a string type, for example:
```
let x = 3.5;
let s = tostring x; // s is "3.5"

let truth = tostring true; // "true"
let fake = tostring false; // "false"
let empty = tostring nil; // "nil"
```

### String Concatenation
Pongo has a separate string concatenation operator taken from Lua, `..`, used to concatenate strings:
```
let msg = "Hello " .. "world!";
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

Note that these do enforce type-checking using the language's type system, so `"3" == 3` and `"true" == true` both output `false`, as differently typed variables are *never* equivalent. Also, checking if a variable is null or unassigned is as easy as `x == nil`.

### Logical Operators
Logical operators act as keywords in order to be more readable, similar to python and GDScript, but work pretty much the same as they do in every other language, operating on Boolean variables and outputting a Boolean variable. For example:
```
not true;
true and false;
true or false;
```

### Precedence and Grouping
Pongo's rules regarding operator precedence are the same as C's. You can also group statements using parentheses:
```
let average = (min + max) / 2;
```


## Statements
Statements in Pongo can consist of the expressions mentioned before terminated by `;` (a.k.a. expression statements).

<!-- TODO insert statement about blocks with { } and mention scoping -->

## Variables
As I said earlier, primitive-type variables are defined with `let`, and arrays are defined with the bracket notation. Variables can be declared without being defined, with primitive variables being initially defined as `nil` and arrays being initially filled with `nil`:
```
let x = 3;
let y; // y = nil;
```
All variables in Pongo have global scope.
<!-- TODO come up with way to have mutable vars -->
<!--
```
let arr1[3] = [3, 4, 5];
let arr2[3]; // arr2[3] = [nil, nil, nil];
```
-->


## Control Flow

As is evident with the name, Pongo is all about a rather messy control flow, 'ping-ponging' around your code file. To do this, Pongo uses labels and goto's for its primary control flow.

### Labels
Pongo defines labels in a code block with the keyword `lbl`, like so:
```
lbl test {
	let x = 3.5;
	print (tostring x);
}; // notice the ';', this is important!
```

Label blocks are terminated by a `;`. Optionally, after the block's closing `}` but before the `;`, you can put in the name of another label to jump to after this block ends. If no label is provided, the program will continue onward (line by line) immediately after the label block. If the user puts "exit" here instead, after executing this block, the program will exit. For example:
```
// program starts at the top

let x = 3;
let y = 2;
let z;

lbl test {
	z = x * y;
} main; // will jump to label "main" after executing this block's instructions

lbl ignored {
	z = x + y; // this block is ignored as the program jumps to "main".
};

lbl main {
	print ("Final output: " .. (tostring z))
} exit; // program exits, printing "6"
```
