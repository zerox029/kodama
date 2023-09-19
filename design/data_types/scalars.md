# Scalar types

## Integers
| Size    | Signed | Unsigned |
|---------|--------|----------|
| 8-bit   | i8     | u8       |
| 16-bit  | i16    | u16      |
| 32-bit  | i32    | u32      |
| 64-bit  | i64    | u64      |
| 128-bit | i128   | u128     |

All integer literals can use the character `_` as a visual separator of units. This is purely visual and has no effect on the compilation result. For example, the number `1000000` can be written as `1_000_000` to facilitate reading.

## Floats
| Size    | Type |
|---------|------|
| 32-bit  | f32  |
| 64-bit  | f64  |

All floating-point data types are signed.

Like integer literals, all floating-point literals can use the character `_` as a visual separator of units. This is purely visual and has no effect on the compilation result. For example, the number `1000000.00` can be written as `1_000_000.00` to facilitate reading.

## Booleans
Boolean values are denoted with the dataType `bool` and can have two values, `true` or `false`. When casted to a number, these evaluate to `1` and `0` respectively.

## Characters
Characters are denoted with the dataType `char` and are specified with single quotes in the following way: `let x: char = 'a'`. Each char can hold 16 bits of data.