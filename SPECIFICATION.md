# Pongo

Pongo is a 16-bit interpreted programming language, executing statement-by-statement unless the control flow is broken by a goto (the control flow "ping-ponging" all over the place, hence the name).

## Syntax
Pongo is a C-like language, using semicolons, parentheses for grouping, mixed-case variable names with underscores, and not caring very much about whitespace. The key difference is that comments are in the same form as Python's, with `#` marking an inline comment.

## Expressions and Statements
Pongo programs consist of a series of statements, each terminated with a semicolon.

### Valid Statements
The following is an exhaustive list of all valid statements in Pongo, with quick descriptions of what they do (with parentheses indicating that something is optional):
 - `short name (= expression)`: Declare a new short and (optionally) assign it the value of expression.
 - `buff name = expression`: Declare a new buff and set its size to the value of expression.
 - `lbl name (= integer)`: Declare a new label and optionally set its value to an integer.
 - `exit`: Exit the program.
 - `smash name`: Smash/de-scope the variable (short or buff) called "name".
 - `clock`: Print the current elapsed runtime of the program in seconds.
 - `if expression goto name`: Jump to the label "name" if the expression evaluates to true or -1.
 - `goto name`: Unconditionally jump to the label "name".
 - `name (@ index) = expression`: Reassign the existing variable "name" to the evaluated expression only if "name" is a short. If the "@ index" is present, and the "name" refers to a buff, then the element at the index will be assigned. Otherwise, if the "name" is a short, then if the expression evaluates to true (-1), the bit indexed by index will be set to 1, and if it evaluates to false (0) the bit indexed by index will be set to 0. If the "name" is a short and the "@ index" is present, and the expression doesn't evaluate to true or false, a fatal error occurs.
 - `smashall`: Smash/de-scope every currently existing variable in the program (not labels).
 - `print expression`: Print an evaluated expression without a new-line character at the end.
 - `print stringliteral`: Print a text message without a new-line character at the end
 - `println (expression or stringliteral)`: Print an evaluated expression or a text message on its own line. If neither are provided, an empty line is printed.

### Expression Operators
Here is a quick-and-dirty exhaustive list of all expression operators, in order of precedence (going from lowest precedence to highest precedence):
 - `|`, bitwise OR
 - `&`, bitwise AND
 - `==` and `!=`, doing the same things they do in C (but returning -1 for true and 0 for false)
 - `<=`, `>=`, `<`, and `>`, also doing the same things they do in C (but returning -1 for true and 0 for false)
 - binary `+` (addition) and `-` (subtraction), like in C
 - `*` (multiplication), `/` (integer division), and `%` (modulo), also like C
 - unary prefix operators `-` (negate), `+` (positive), `!` (bitwise NOT), and `sizeof`
 - `@` (at), an "index" operator

Nearly all expression operators are taken from C, with some exceptions and changes in behavior:
 - The unary operator `sizeof` will return 16 when performed on a short or label (as it's 16 bits) but will return the length of a buff when used on a buff
 - The unary plus operator will attempt to convert any negative value to its positive equivalent, but beware of overflows causing undefined behavior
 - The `@` operator is... something. See the next section.

#### The @ operator
The @ operator has the highest precedence over all other operators, comparable in precedence to the `.` in C for accessing struct members. This operator comes in the form `identifier @ index`. 

If the identifier is for a short or label, it accesses the binary digit specified by the index (0-15, with 0 being the 1's place), returning true (-1) if the digit is a one or false (0) if it is a 0. 

If the identifier is for a buff, it accesses the element (a short) at that index.

### Special keywords
Pongo utilizes two special keywords for expression evaluation.

`input` is placed in an expression and has the same precedence as a variable name, which scans the standard input for a single short and returns it before evaluating the rest of the expression.

`rand` is similar to input, only instead of reading a number from the standard input, it generates a new pseudorandom 16-bit integer with the same bounds as a normal int16 (so between -32768 and 32767, inclusive).

## Data Types
### Labels
Pongo labels are declared using `lbl` followed by the name of the label. Labels are immutable 16-bit integers. Here is an example:
```
lbl FOO;

lbl BAR = 3;

lbl BAZ = 0xffff;
```

When a label is declared but not assigned a particular value (in the case of FOO), the value is set to the current statement number in the program (with the first statement being 0). Note that hex literals (like in C) are valid pongo.

If you do assign the label a specific value, it can only be assigned positive integer values between 0 and 32767 (inclusive). Labels are globally accessible for any line of the program, even if it appears the label has not yet been declared, so this is completely valid pongo:
```
println BAZ;

lbl BAZ = 3;
```

### Shorts
Pongo shorts are declared using `short` followed by a name. These are mutable 16-bit signed integers. Here is an example:
```
short x;
short y = 33;
short z = y + 2;
```

These essentially work exactly the same as C's short datatype. Unlike labels, shorts can be reassigned, and (obviously) don't have to only be assigned integer literals:
```
short z = 13;
z = 12;
```

Shorts cannot be declared twice, so the following will exit with a fatal error:
```
short z;
short z = 13;
```

Shorts, using the `@` operator, can even have their bits flipped. For example:
```
short test = 13; # "01101" in binary
test @ 2 = 0; # now test is "01001", or 9 in decimal
test @ 1 = true; # now test is "01011", or 11 in decimal
```

### Buffs
Pongo has another datatype called a "buff" (short for "buffer") which is pretty much just an array of mutable 16-bit integers. Buffs are declared as follows:
```
buff this_is_my_boomstick = 4;
```
With the value after the `=` being the initial size of the buffer. The code example above creates an array of 4 16-bit integers, indexed 0-3. The elements are initialized to 0s.

Buffs cannot be resized after being initialized.

Buff elements can be modified using the `@` ("at") operator, as follows:

```
buff newbuff = 3;

newbuff @ 0 = -1;
newbuff @ 1 = -2;
newbuff @ 2 = 42;
```

### "Booleans" (but not really)
Pongo does not actually have a boolean datatype, but it does use `true` and `false` as boolean-esque macros, evaluating to -1 and 0 respectively:
```
short b = true;
println b; # will print -1
```

The reason for this is that in boolean algebra, the conjunction of some boolean variable and "true" should be equivalent to the original boolean variable. As pongo uses bitwise operators, the 16-bit integer that would produce these properties when taking the conjunction of it and some other variable is -1 (or 0xffff).

Any comparison operators by extension will return -1 when their conditions are satisfied, and 0 when they aren't.

### String Literals
Pongo does not actually have a string type, but string literals can be passed to the print 

## Memory and Scoping
Pongo doesn't really use scope like other programming languages. Labels are globally accessible at any point in the program, and shorts and buffs are accessible for every statement executed after their declaration. A bit hard to keep track of, but that's what happens in non-structured programming.

To combat this, pongo uses the `smash` keyword. Buffs and shorts can be "smashed" in order to de-scope them and remove them from the program's environment, essentially rendering them forgotten and free to be re-declared again. For example:

```
short x = 3; # from here until x is smashed, it is accessible
buff z = 4;

short y = x;

smash x; # now x ceases to exist
smash z; # same for z

short z; # since z was smashed, it can be redeclared as something else

y = x + 1; # this will exit the program with an error as x no longer exists
```

There also exists the `smashall` statement, which will smash all variables (again, not labels) that currently exist in the program's environment.

## Control Flow
Control flow in pongo is accomplished through gotos. Here is a simple idiom for a loop that prints the numbers 0 to 9:
```
short x = 0;

lbl LOOP;
  println x;
  x = x + 1;
if x < 10 goto LOOP;

smash x; # might as well smash x once we're done with it
```

Conditional gotos appear as "if (condition) goto (label)", and unconditional gotos appear simply as "goto (label)". Only labels can be passed as the destination for gotos, and after jumping to the label the program continues executing from that point.

Conditional gotos will only jump to the specified label if the condition evaluates to true (i.e. -1)