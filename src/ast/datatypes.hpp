//
// Created by emma on 16/09/23.
//

#ifndef KODAMA_SRC_AST_DATATYPES_HPP_
#define KODAMA_SRC_AST_DATATYPES_HPP_

#include "../lexer/lexer.hpp"
#include <unordered_map>

enum DataType {
  U8,
  U16,
  U32,
  U64,
  U128,
  I8,
  I16,
  I32,
  I64,
  I128
};

DataType
TokenTypeToDataType(TokenType tokenType);

bool
IsUnsigned(DataType dataType);

#endif //KODAMA_SRC_AST_DATATYPES_HPP_