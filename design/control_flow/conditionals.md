# Conditionals

`if` and `else` provide conditional execution of statements. The syntax is:

> `if (`_boolean expression_ `) {` _statements_ `}`
>
> [ `else if (` _boolean expression_ `) {` _statements_ `}` ] ...
>
> [ `else {` _statements_ `}` ]

Only one group of statements will execute:

-   When the first `if`'s boolean expression evaluates to true, its associated
    statements will execute.
-   When earlier boolean expressions evaluate to false and an `else if`'s
    boolean expression evaluates to true, its associated statements will
    execute.
    -   `... else if ...` is equivalent to `... else { if ... }`, but without
        visible nesting of braces.
-   When all boolean expressions evaluate to false, the `else`'s associated
    statements will execute.

When a boolean expression evaluates to true, no later boolean expressions will
evaluate.

Note that `else if` may be repeated.

For example:

```
if (fruit.isYellow()) {
  print("Banana!");
} else if (fruit.isOrange()) {
  print("Orange!");
} else if (fruit.isGreen()) {
  print("Apple!");
} else {
  print("Vegetable!");
}
```