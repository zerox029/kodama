# Other operators
| Operators | Description              |
| --------- |--------------------------|
| `?:`      | Ternary conditional      |
| `?.`      | Optional chaining        |
| `&`       | Address-of               |
| `*`       | Dereference              |
| `.`       | Member access            |
| `...`     | Range literal            |
| `??`      | Null coalescing operator |

## Ternary conditional
Used in the form `a ? b : c`, it is a shorthand for `if...else` statements. `a` must be a condition, after getting executed, `b` will be the expression to execute if `a` is truthy and `c` if `a` is falsy.

## Optional chaining
Used in the form `a?.b`, it performs a member access of `b` on `a` if and only if `a` exists and is non-null. IfElseStatement `a` cannot be accessed, the operator short-circuits gracefully and `nil` is returned.

## Address-of
Used in the form `&a`, it returns the memory address of the variable `a`.

## Dereference
Used in the form `*a`, it returns the value located at the memory address `a`.

## Member access
Used in the form `a.b`, it retrieves a member `a` of a structure `b`. IfElseStatement `a` is null, it throws a (TODO: flesh out the exception system). To avoid this behavior, use the optional chaining `?.` operator.

## Range literal
Used in the form `a...b`, it returns a sequence of integers ranging from `a` to `b` exclusively, in the form of an array such that the expression `0...5` returns the array `[0, 1, 2, 3, 4]`.

## Null coalescing operator
Used in the form `a ?? b`, it returns the first non-null value between `a` and `b`. For example, if `a = 5` and `b = 10`, then `a ?? b` would return `5`. However, if `a = null` and `b = 10`, then `a ?? b` would return `10`.