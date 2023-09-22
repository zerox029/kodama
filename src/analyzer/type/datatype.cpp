//
// Created by emma on 22/09/23.
//

#include "datatype.hpp"
#include "../../ast/ast.hpp"

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
    {TK_BOOL, std::make_shared<BoolType>()}
};

/// U8
TypeCategory
U8Type::GetTypeCategory() const { return UINTEGER; }

llvm::Type*
U8Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt8Ty(context); }

AstNodePtr
U8Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U8Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U16
TypeCategory
U16Type::GetTypeCategory() const { return UINTEGER; }

llvm::Type*
U16Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt16Ty(context); }

AstNodePtr
U16Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U16Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U32
TypeCategory
U32Type::GetTypeCategory() const { return UINTEGER; }

llvm::Type*
U32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt32Ty(context); }

AstNodePtr
U32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U64
TypeCategory
U64Type::GetTypeCategory() const { return UINTEGER; }

llvm::Type*
U64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt64Ty(context); }

AstNodePtr
U64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// U128
TypeCategory
U128Type::GetTypeCategory() const { return UINTEGER; }

llvm::Type*
U128Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt128Ty(context); }

AstNodePtr
U128Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
U128Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}

/// I8
TypeCategory
I8Type::GetTypeCategory() const { return INTEGER; }

llvm::Type*
I8Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt8Ty(context); }

AstNodePtr
I8Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I8Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I16
TypeCategory
I16Type::GetTypeCategory() const { return INTEGER; }

llvm::Type*
I16Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt16Ty(context); }

AstNodePtr
I16Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I16Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I32
TypeCategory
I32Type::GetTypeCategory() const { return INTEGER; }

llvm::Type*
I32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt32Ty(context); }

AstNodePtr
I32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I64
TypeCategory
I64Type::GetTypeCategory() const { return INTEGER; }

llvm::Type*
I64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt64Ty(context); }

AstNodePtr
I64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// I128
TypeCategory
I128Type::GetTypeCategory() const { return INTEGER; }

llvm::Type*
I128Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt128Ty(context); }

AstNodePtr
I128Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0");
}

bool
I128Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// F32
TypeCategory
F32Type::GetTypeCategory() const { return DECIMAL; }

llvm::Type*
F32Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getFloatTy(context); }

AstNodePtr
F32Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0", "0");
}

bool
F32Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// F64
TypeCategory
F64Type::GetTypeCategory() const { return DECIMAL; }

llvm::Type*
F64Type::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getDoubleTy(context); }

AstNodePtr
F64Type::GetDefaultValue() const {
  return std::make_shared<NumberLiteral>("0", "0");
}

bool
F64Type::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
}


/// Bool
TypeCategory
BoolType::GetTypeCategory() const { return BOOL; }

llvm::Type*
BoolType::GetLLVMType(llvm::LLVMContext& context) const { return llvm::Type::getInt1Ty(context); }

AstNodePtr
BoolType::GetDefaultValue() const {
  return std::make_shared<BoolValue>(false);
}

bool
BoolType::IsCastableTo(Datatype& castType) const {
  return IsNumericType(castType);
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