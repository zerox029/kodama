# Type inference

## Overview
When a variable or a function is not specified a data type, the compiler will assign it one by inferring the closest possible type given the context. The only exception to this is for function parameters where a data type must be specified.
When multiple sizes of the same data type are available, for example with integer or floating point values, the compiler will always default to the 64 bit version. Similarily, when signed and unsigned alternatives exist, the signed version will be favored given a positive value. For example, given the expression `let x = 10`, the inferred datatype for `x` will be `i64`;

## Alternatives considered
- Using the keyword `auto` or `_` to signify type inference instead of leaving the space blank