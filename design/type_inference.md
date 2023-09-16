# Type inference

## Overview
When a variable or a function is not specified a data dataType, the compiler will assign it one by inferring the closest possible dataType given the context. The only exception to this is for function parameters where a data dataType must be specified.
When multiple sizes of the same data dataType are available, for example with integer or floating point values, the compiler will always default to the 64 bit version. Similarily, when signed and unsigned alternatives exist, the signed version will be favored given a positive value. For example, given the expression `let x = 10`, the inferred datatype for `x` will be `i64`;

## Alternatives considered
- Using the keyword `auto` or `_` to signify dataType inference instead of leaving the space blank