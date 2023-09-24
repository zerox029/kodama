//
// Created by emma on 22/09/23.
//

#include "datatype.hpp"
#include "../../ast/ast.hpp"
#include "llvm/IR/Constants.h"

const std::unordered_map<TokenType, TypePtr> dataTypeMapping{
    {TK_U8, std::make_shared<U8Type>()},
    {TK_U16, std::make_shared<U16Type>()},
    {TK_U32, std::make_shared<U32Type>()},
    {TK_U64, std::make_shared<U64Type>()},
    {TK_U128, std::make_shared<U128Type>()},
    {TK_I8, std::make_shared<I8Type>()},
    {TK_I16, std::make_shared<I16Type>()},
    {TK_I32, std::make_shared<I32Type>()},
    {TK_I64, std::make_shared<I64Type>()},
    {TK_I128, std::make_shared<I128Type>()},
    {TK_F32, std::make_shared<F32Type>()},
    {TK_F64, std::make_shared<F64Type>()},
    {TK_BOOL, std::make_shared<BoolType>()},
    {TK_STRING, std::make_shared<StringType>()}
};

/// U8_TYPE
TypeCategory
U8Type::GetTypeCategory() const { return UINTEGER; }

TypeName
U8Type::GetTypeName() const { return U8_TYPE; }

std::string
U8Type::GetTypeNameString() const { return "u8"; }

llvm::Type*
U8Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt8Ty(context); }

AstNodePtr
U8Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
U8Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U16_TYPE
TypeCategory
U16Type::GetTypeCategory() const { return UINTEGER; }

TypeName
U16Type::GetTypeName() const { return U16_TYPE; }

std::string
U16Type::GetTypeNameString() const { return "u16"; }

llvm::Type*
U16Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt16Ty(context); }

AstNodePtr
U16Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
U16Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U32_TYPE
TypeCategory
U32Type::GetTypeCategory() const { return UINTEGER; }

TypeName
U32Type::GetTypeName() const { return U32_TYPE; }

std::string
U32Type::GetTypeNameString() const { return "u32"; }

llvm::Type*
U32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt32Ty(context); }

AstNodePtr
U32Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
U32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U64_TYPE
TypeCategory
U64Type::GetTypeCategory() const { return UINTEGER; }

TypeName
U64Type::GetTypeName() const { return U64_TYPE; }

std::string
U64Type::GetTypeNameString() const { return "u64"; }

llvm::Type*
U64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt64Ty(context); }

AstNodePtr
U64Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
U64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U128_TYPE
TypeCategory
U128Type::GetTypeCategory() const { return UINTEGER; }

TypeName
U128Type::GetTypeName() const { return U128_TYPE; }

std::string
U128Type::GetTypeNameString() const { return "u128"; }

llvm::Type*
U128Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt128Ty(context); }

AstNodePtr
U128Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
U128Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}

/// I8_TYPE
TypeCategory
I8Type::GetTypeCategory() const { return INTEGER; }

TypeName
I8Type::GetTypeName() const { return I8_TYPE; }

std::string
I8Type::GetTypeNameString() const { return "i8"; }

llvm::Type*
I8Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt8Ty(context); }

AstNodePtr
I8Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
I8Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I16_TYPE
TypeCategory
I16Type::GetTypeCategory() const { return INTEGER; }

TypeName
I16Type::GetTypeName() const { return I16_TYPE; }

std::string
I16Type::GetTypeNameString() const { return "i16"; }

llvm::Type*
I16Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt16Ty(context); }

AstNodePtr
I16Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
I16Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I32_TYPE
TypeCategory
I32Type::GetTypeCategory() const { return INTEGER; }

TypeName
I32Type::GetTypeName() const { return I32_TYPE; }

std::string
I32Type::GetTypeNameString() const { return "i32"; }

llvm::Type*
I32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt32Ty(context); }

AstNodePtr
I32Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
I32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I64_TYPE
TypeCategory
I64Type::GetTypeCategory() const { return INTEGER; }

TypeName
I64Type::GetTypeName() const { return I64_TYPE; }

std::string
I64Type::GetTypeNameString() const { return "i64"; }

llvm::Type*
I64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt64Ty(context); }

AstNodePtr
I64Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
I64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I128_TYPE
TypeCategory
I128Type::GetTypeCategory() const { return INTEGER; }

TypeName
I128Type::GetTypeName() const { return I128_TYPE; }

std::string
I128Type::GetTypeNameString() const { return "i128"; }

llvm::Type*
I128Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt128Ty(context); }

AstNodePtr
I128Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
I128Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// F32_TYPE
TypeCategory
F32Type::GetTypeCategory() const { return DECIMAL; }

TypeName
F32Type::GetTypeName() const { return F32_TYPE; }

std::string
F32Type::GetTypeNameString() const { return "f32"; }

llvm::Type*
F32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getFloatTy(context); }

AstNodePtr
F32Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
F32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// F64_TYPE
TypeCategory
F64Type::GetTypeCategory() const { return DECIMAL; }

TypeName
F64Type::GetTypeName() const { return F64_TYPE; }

std::string
F64Type::GetTypeNameString() const { return "f64"; }

llvm::Type*
F64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getDoubleTy(context); }

AstNodePtr
F64Type::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
F64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// Bool
TypeCategory
BoolType::GetTypeCategory() const { return BOOLEAN; }

TypeName
BoolType::GetTypeName() const { return BOOL_TYPE; }

std::string
BoolType::GetTypeNameString() const { return "bool"; }

llvm::Type*
BoolType::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt1Ty(context); }

AstNodePtr
BoolType::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "0", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "0");
}

bool
BoolType::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// String type
TypeCategory
StringType::GetTypeCategory() const { return STRING; }

TypeName
StringType::GetTypeName() const { return STRING_TYPE; }

std::string
StringType::GetTypeNameString() const { return "string"; }

llvm::Type*
StringType::GetLLVMType(llvm::LLVMContext& context) const { return llvm::ArrayType::get(llvm::Type::getInt8Ty(context), 10); }

AstNodePtr
StringType::GetDefaultValue() const {
  Token dummyToken{TK_NUMBER, "", {"", 0, 0}};
  return std::make_shared<IntegerLiteral>(dummyToken, "");
}

bool
StringType::IsCastableTo(Datatype& castType) const {
  return false;
}


TypePtr
TokenTypeToDataType(TokenType tokenType) {
  return dataTypeMapping.at(tokenType);
}

bool
IsNumericType(Datatype& dataType) {
  return dataType.GetTypeCategory() == INTEGER
      || dataType.GetTypeCategory() == UINTEGER
      || dataType.GetTypeCategory() == DECIMAL;
}