//
// Created by emma on 16/09/23.
//

#include "datatypes.hpp"
#include "../parser/parser.hpp"

DataType
TokenTypeToDataType(TokenType tokenType) {
  return dataTypeMapping.at(tokenType);
}