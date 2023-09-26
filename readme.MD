# The Kodama programming language
Kodama is a modern and friendly, strongly typed systems programming language.
It is still heavily under development and not meant for end-user use.

## Overview
- Type inference
- First-class functions
- Algebraic data types
- Parametric polymorphism
- Pattern matching

## Safety
By default, integer overflows will cause runtime errors. To specify that a variable can overflow, it must be marked with the `[[can_overflow]]` attribute
```
let x: u8 = 255
x++ // Will cause a runtime error

[[can_overflow]] let y: u8 = 255
x++ // Will overflow without causing an error
```

## Values and Variables
Values represent the result of an expression. They are immutable and are declared with the `val` keyword.
```
// Creates an mutable variable x with value 5
val x = 5+5
x = 20-5 // This will not work
```

Variables in Kodama are declared using the `let` keyword. Unlike values, they can be freely reassigned to

```
// Creates an mutable variable x with value 5
let x = 5+5
x = 20-5 // This will work
```

Data types can be specified by apending them to the variable name but will also be inferred by the compiler if left unspecified. 
The keyword `auto` can also be used to let the compiler infer the required dataType.
When multiple sizes of the same data dataType are available, the compiler will always default to 64 bit.
```
// x is infered to be f64
let x = 3.0

// x is infered to be f64 through the use of auto
let x: auto = 3.0

// y is explicitely set to be f32
let x: f32 = 3.0
```

Temporary variables nested within expressions can be created with the `val ... in` syntax. This ensures the variables are scoped to that specific block only.
```
let add =
    val x = 10 in
    val y = 25 in
    x + y
```

Variables are non-nullable by default. They can be made nullable by adding a question mark (?) to the dataType.
**Open question**: Would it be better to use a `[[nullable]]` annotation instead? And get rid of the auto keyword.
```
let x = null // Will not compile
let x: SomeObject? = null // Will compile correctly
```

## Functions declarations
Functions in Kodama are declared using the `fn` keyword. Like variables, return types can be explicitely specified but will be infered by the compiler otherwise. Arguments however must have a specified dataType.
```
// Declare a function five
fn five() {
    return 5
}

// Declare a function floatFive explicitely setting its return dataType to f32
fn floatFive() -> f32 {
    return 5.0
}

// Declare a function addOne that takes in a u64
fn addOne(x: u64) {
    return x + 1
}

// One liner return shorthand
fn addOne(x: u64) => x + 1
```

## Function calls
Functions can be called by specifying each argument in the following way
```
setSize(20, 50)
```

Arguments can also be specified a name which allows them to be rearranged within the function call. If a function call contains both named and unnamed arguments, the unnamed arguments must come first and in the order specified in the function declaration.
```
setSize(width: 20, height: 50)
setSize(20, height: 50) // Works
setSize(width:20, 50) // Does not work
```

## Classes and structs
Structures in Kodama can be created with either `struct` or `class`

### `struct`
Members of a struct are public by default.
```
struct Rectangle {
    width: i64
    height: i64
}
```
Structs in Kodama are passed by value. Copying a struct instance will always produce a deep copy such that any variable containing one will possess a unique instance.

### `class`
Members of a class are by default assumed to be private, the `public` identifier must be added otherwise. \
Member variables can be accessed using `self` notation or with the shorthand `.` ommiting the `self`.
```
class Rectangle {
    width: i64
    height: i64

    public fn area() -> i64 => return .width * .height
}
```

A default constructor will be generated for classes, it takes all fields by name. For the above class, the following will get generated.
```
Rectangle(width: 10, height: 20)
```
Custom constructors can be created in the following way
```
public Rectangle(width: i64, height: i64) -> Rectangle {
    .width = width
    .height = height
}
```

Classes in Kodama are passed by reference such that copying a class copies a reference to that class and two variables may point to the same instance of a single class.


### `mixin`
Todo

## Control flow
Kodama supports 3 main control flow statements:

- `if ... (else if) ... (else)`
```
let num = 5
if(num == 5) {
    print("This executes")
} else if (num == 10) {
    print("This does not execute")
} else {
    print("This also does not execute")
}
```

- `(do) ... while`
```
// Prints all numbers from 0 to 10
let x = 0
while(x <= 10) {
    print(x)
    x++
}

// Prints 0
let x = 0
do {
    print(x)
    x++
} while(x < 0)
```

- `for`
```
// Prints numbers from 0 to 5 inclusively
for value in 0 to 5 {
    print(value)
}

// Prints numbers from 0 to 5 exclusively
for value in 0 until 5 {
    print(value)
}

// Prints all values from a given list that are greater than 10
for value in list where value > 10 {
    print value
}
```

## Arrays
Arrays are dynamically sized and can grow and shrink at runtime. \
**Open question**: Do we really want that behavior?

```
// Function that takes an array of u64 and returns an array of strings
fn foo(numbers: [u64]) -> [String] {
    ...
}

// Creating an array of 5 integers
let array: [i64] = [1, 2, 3, 4, 5] 
```
An array spanning a continuous range of integers can be created with the following shorthand
```
// Create an i64 array from 0 to 100 in leaps of 2
let range: [i64] = 0...100:2
```

## Sets
Sets can be created and accessed in the following way
```
let set: {String} = {"one", "two", "three"}
print(set.contains("one")) // prints true
```

## Records
Records can be created an accessed in the following way

```
let record: {String:i64} = {"one": 1, "two: 2, "three": 3}
print(record["one"]) // prints 1
```

## Tuples
Tuples can be created and accessed in the following way. Since tuples are immutable, they must be created with `val`, not `let`.
```
val tuple: (String, bool, u64) = ("heya", true, 55)
print(tuple[1]) // prints true
```
