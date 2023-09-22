//
// Created by emma on 22/09/23.
//

#include "datatype.hpp"

TypeCategory
TokenTypeToDataType(TokenType tokenType) {
  return dataTypeMapping.at(tokenType);
}

bool
IsUnsigned(TypeCategory dataType) {
  return dataType == U8 || dataType == U16 || dataType == U32 || dataType == U64 || dataType == U128;
}
