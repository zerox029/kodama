//
// Created by emma on 22/09/23.
//

#ifndef KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_
#define KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_

#include "../../lexer/token.hpp"
#include <memory>
#include <unordered_map>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>

class AstNode;
typedef std::shared_ptr<AstNode> AstNodePtr;

enum TypeCategory {
  STRUCT,
  UINTEGER,
  INTEGER,
  DECIMAL,
  BOOLEAN,
  STRING
};

enum TypeName {
  STRUCT_TYPE,
  U8_TYPE,
  U16_TYPE,
  U32_TYPE,
  U64_TYPE,
  U128_TYPE,
  I8_TYPE,
  I16_TYPE,
  I32_TYPE,
  I64_TYPE,
  I128_TYPE,
  F32_TYPE,
  F64_TYPE,
  BOOL_TYPE,
  STRING_TYPE
};

class Datatype {
 public:
  virtual TypeCategory GetTypeCategory() const = 0;
  virtual TypeName GetTypeName() const = 0;
  virtual std::string GetTypeNameString() const = 0;
  virtual llvm::Type* GetLLVMType(llvm::LLVMContext& context) const = 0;
  virtual bool IsCastableTo(Datatype& castType) const = 0;
  virtual AstNodePtr GetDefaultValue() const = 0;

  void SetMutability(bool mutability) { isMutable = mutability; }
  bool IsMutable() { return isMutable; }

  bool operator==(const Datatype& type) const {
    return GetTypeName() == type.GetTypeName();
  }

  bool operator!=(const Datatype& type) const {
    return !operator==(type);
  }

 private:
  bool isMutable = true;
};

typedef std::shared_ptr<Datatype> TypePtr;

class StructType : public Datatype {
 private:
  std::string identifier;
  std::vector<std::pair<std::string, TypePtr>> members;

 public:
  StructType(std::string identifier);
  StructType(std::string identifier, std::vector<std::pair<std::string, TypePtr>> members);

  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class BoolType : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class StringType : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  TypeName GetTypeName() const override;
  std::string GetTypeNameString() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};
TypePtr
TokenTypeToDataType(TokenType tokenType);

bool
IsNumericType(Datatype& dataType);

#endif //KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_
