# The Kodama programming language
Kodama is a modern and friendly, strongly typed systems programming language. It is designed to build reliable and maintainable software that without sacrificing on efficiency.

## Type safety
- Immutability by default
- All casts must be explicit

## Math safety
By default, integer overflows will cause runtime errors. To specify that a variable can overflow, it must be marked with the `[[can_overflow]]` attribute
```
let mut x: u8 = 255
x++ // Will cause a runtime error

[[can_overflow]] let mut y: u8 = 255
x++ // Will overflow without causing an error
```

## Variables
Variables in Kodama are declared using the `let` keyword. They are by default immutable and must be appended with the `mut` keyword to make them mutable.

```
// Creates an unmutable variable x with value 5
let x = 5

// Creates a mutable variable y with value 5
let mut y = 5
```

Data types can be specified by apending them to the variable name but will also be infered by the compiler if left unspecified
```
// x is infered to be f64
let mut x = 3.0

// y is explicitely set to be f32
let mut x: f32 = 3.0
```

## Functions declarations
Functions in Kodama are declared using the `def` keyword. Like variables, return types can be explicitely specified but will be infered by the compiler otherwise. Arguments however must have a specified type.
```
// Declare a function five
def five() {
    return 5
}

// Declare a function floatFive explicitely setting its return type to f32
def floatFive() -> f32 {
    return 5.0
}

// Declare a function addOne that takes in a u64
def addOne(x: u64) {
    return x + 1
}

// One liner shorthand
def addOne(x: u64) => return x + 1
```

## Function calls
The name of each argument must be specified when calling functions
```
setSize(width: 20, height: 50)
```
There are two exceptions to this:
- If the parameter in the function declaration is declared as anon, omitting the argument label is allowed.
- When passing a variable with the same name as the parameter.

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
Member variables can be accessed using the `self.` notation or with the shorthand `.` ommiting the `self`.
```
class Rectangle {
    width: i64
    height: i64

    public def area() -> i64 => return .width * .height
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


## Control flow
Kodama supports 3 main control flow statements:

- `if else`
```
let num = 5
if(num == 5) {
    print("This executes")
} else {
    print("This does not execute")
}
```

- `(do) while`
```
let mut counter = 0
while(counter < 5) {
    print(counter)
    counter++
}

let mut counter = 0
do {
    print(counter)
    counter++
} while()
```

- `for`
```
for value in (0...5) {
    print(value)
}
```

## Arrays
Arrays are dynamic in size and can grow and shrink at runtime.

```
// Function that takes an array of u64 and returns an array of strings
def foo(numbers: [u64]) -> [String] {
    ...
}

// Creating an array of 5 integers
let array: [i64] = [1, 2, 3, 4, 5] 

// Creating an array of 100 items initialized at 0
let array: [i64] = [0; 100]
```

## Sets
Sets can be created and accessed in the following way
```
let set: {String} = {"one", "two", "three"}
print(set.contains("one")) // prints true
```

## Dictionaries
Dictionaries can be created an accessed in the following way

```
let dict: {String:i64} = {"one": 1, "two: 2, "three": 3}
print(dict["one"]) // prints 1
```


## Tuples
Tuples can be created and accessed in the following way
```
let tuple: (String:bool:u64) = ("heya", true, 55)
print(tuple[1]) // prints true
```


## Operators

### Arithmetic operators
| Operator | Description    |
| -------- | -------------- |
| `+`      | Addition       |
| `-`      | Substraction   |
| `*`      | Multiplication |
| `/`      | Division       |
| `%`      | Modulo         |
| `++`     | Increment      |
| `--`     | Decrement      |


### Assignment operators
| Operator | Description |
| --- | --- |
| `=` |  Assignment |
| `+=` | Arithmetic addition and assignment |
| `-=` |  Arithmetic substraction and assignment |
| `*=` |  Arithmetic multiplication and assignment |
| `/=` |  Arithmetic division and assignment |
| `%=` |  Arithmetic modulo and assignment |


### Relational operators
| Operators | Description |
| --- | --- |
| `==` | Equality |
| `!=` | Inequality |
| `>` | Greater than |
| `<` | Less than |
| `>=` | Greater than or equal to |
| `<=` | Less than or equal to |

### Logical operators
| Operators | Description |
| --- | --- |
| `&&` | Logical AND |
| `\|\|` | Logical OR |
| `!` | Logical complement |

### Bitwise operators
| Operators | Description |
| --- | --- |
| `&` | Bitwise AND |
| `\|` | Bitwise OR |
| `^` | Bitwise XOR |
| `~` | Bitwise NOT |
| `<<` | Bitwise left-shift |
| `>>` | Bitwise right-shift |

### Other operators
| Operators | Description |
| --- | --- |
| `?:` | Ternary conditional |
| `?.` | Optional chaining |
| `&` | Address-of |
| `.` | Member access |
| `*` | Dereference |
| `...` | Range literal |
| `??` | Null coalescing operator |