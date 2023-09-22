//
// Created by emma on 22/09/23.
//

#include "datatype.hpp"
#include "../../ast/ast.hpp"

/// U8
TypeCategory
U8Type::GetTypeCategory() const { return U8; }

AstNodePtr
U8Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U8Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// U16
TypeCategory
U16Type::GetTypeCategory() const { return U16; }

AstNodePtr
U16Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U16Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// U32
TypeCategory
U32Type::GetTypeCategory() const { return U32; }

AstNodePtr
U32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U32Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// U64
TypeCategory
U64Type::GetTypeCategory() const { return U64; }

AstNodePtr
U64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U64Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// U128
TypeCategory
U128Type::GetTypeCategory() const { return U128; }

AstNodePtr
U128Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U128Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}

/// I8
TypeCategory
I8Type::GetTypeCategory() const { return I8; }

AstNodePtr
I8Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I8Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// I16
TypeCategory
I16Type::GetTypeCategory() const { return I16; }

AstNodePtr
I16Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I16Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// I32
TypeCategory
I32Type::GetTypeCategory() const { return I32; }

AstNodePtr
I32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I32Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// I64
TypeCategory
I64Type::GetTypeCategory() const { return I64; }

AstNodePtr
I64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I64Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// I128
TypeCategory
I128Type::GetTypeCategory() const { return I128; }

AstNodePtr
I128Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I128Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// F32
TypeCategory
F32Type::GetTypeCategory() const { return F32; }

AstNodePtr
F32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0", "0");
}

bool
F32Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// F64
TypeCategory
F64Type::GetTypeCategory() const { return F64; }

AstNodePtr
F64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0", "0");
}

bool
F64Type::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


/// F64
TypeCategory
BoolType::GetTypeCategory() const { return BOOL; }

AstNodePtr
BoolType::GetDefaultValue() const {
  /// TODO: Create boolean literals
  return std::make_shared<NumberLiteral>("0", "0");
}

bool
BoolType::IsCastableTo(TypeCategory castType) const {
  return IsNumericType(castType);
}


TypeCategory
TokenTypeToDataType(TokenType tokenType) {
  return dataTypeMapping.at(tokenType);
}

bool
IsUnsigned(TypeCategory dataType) {
  return dataType == U8 || dataType == U16 || dataType == U32 || dataType == U64 || dataType == U128;
}

bool
IsNumericType(TypeCategory dataType) {
  return dataType == U8 || dataType == U16 || dataType == U32 || dataType == U64 || dataType == U128
      || dataType == I8 || dataType == I16 || dataType == I32 || dataType == I64 || dataType == I128
      || dataType == F32 || dataType == F64;
}