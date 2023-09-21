//
// Created by emma on 16/09/23.
//

#include "datatypes.hpp"
#include "../parser/parser.hpp"

const std::unordered_map<TokenType, DataType> dataTypeMapping{
    {TK_U8, U8},
    {TK_U16, U16},
    {TK_U32, U32},
    {TK_U64, U64},
    {TK_U128, U128},
    {TK_I8, I8},
    {TK_I16, I16},
    {TK_I32, I32},
    {TK_I64, I64},
    {TK_I128, I128},
    {TK_F32, F32},
    {TK_F64, F64},
    {TK_BOOL, BOOL}
};

DataType
TokenTypeToDataType(TokenType tokenType) {
  return dataTypeMapping.at(tokenType);
}

bool
IsUnsigned(DataType dataType) {
  return dataType == U8 || dataType == U16 || dataType == U32 || dataType == U64 || dataType == U128;
}