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
  UINTEGER,
  INTEGER,
  DECIMAL,
  BOOL
};

class Datatype {
 public:
  virtual TypeCategory GetTypeCategory() const = 0;
  virtual llvm::Type* GetLLVMType(llvm::LLVMContext& context) const = 0;
  virtual bool IsCastableTo(Datatype& castType) const = 0;
  virtual AstNodePtr GetDefaultValue() const = 0;

  virtual bool operator==(const Datatype& type) const {
    return GetTypeCategory() == type.GetTypeCategory();
  }

  bool operator!=(const Datatype& type) const {
    return !operator==(type);
  }
};

typedef std::shared_ptr<Datatype> TypePtr;

class U8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class BoolType : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  llvm::Type* GetLLVMType(llvm::LLVMContext& context) const override;
  bool IsCastableTo(Datatype& castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

TypePtr
TokenTypeToDataType(TokenType tokenType);

bool
IsNumericType(Datatype& dataType);

#endif //KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_
