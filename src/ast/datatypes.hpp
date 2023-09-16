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
  U128
};

const std::unordered_map<TokenType, DataType> dataTypeMapping{
    {TK_U8, U8},
    {TK_U16, U16},
    {TK_U32, U32},
    {TK_U64, U64},
    {TK_U128, U128},
};

DataType
TokenTypeToDataType(TokenType tokenType);

#endif //KODAMA_SRC_AST_DATATYPES_HPP_