# Loops

## `while` loops
Statements within a while loop will execute for as long the condition is evaluated to `true`. IfElseStatement preceded with a `do`, the statements in the block will evaluate once before evaluating the condition, after which the behaviour is identical to a regular `while` loop. \
For example:

```
// Prints all numbers from 0 to 10
let mut x = 0
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


## `for loops`
`for` statements loop over a given array storing the value at each step in a specified variable. \
For example
```
for value in (0...5) {
    print(value)
}
```

## `continue`
The `continue` statement will skip over the current iteration of a loop and will automatically move on to the Next. \
For example
```
// Prints all numbers from 0 to 10 but skips 5
let mut x = 0
while(x <= 10) {
    if(x == 5)
        continue

    print(x)
    x++
}
```

## `break`
The `break` statement will break out of a loop entirely, bypassing any iterations left. \
For example:
```
// Prints all numbers from 0 to 4
let mut x = 0
while(x <= 10) {
    if(x == 5)
        break

    print(x)
    x++
}
```