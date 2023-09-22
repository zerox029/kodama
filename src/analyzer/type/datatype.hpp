//
// Created by emma on 22/09/23.
//

#ifndef KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_
#define KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_

#include "../../lexer/token.hpp"
#include <memory>
#include <unordered_map>

class AstNode;
typedef std::shared_ptr<AstNode> AstNodePtr;

enum TypeCategory {
  U8,
  U16,
  U32,
  U64,
  U128,
  I8,
  I16,
  I32,
  I64,
  I128,
  F32,
  F64,
  BOOL
};

const std::unordered_map<TokenType, TypeCategory> dataTypeMapping{
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

class Datatype {
 public:
  virtual TypeCategory GetTypeCategory() const = 0;
  virtual bool IsCastableTo(TypeCategory castType) const = 0;
  virtual AstNodePtr GetDefaultValue() const = 0;

  virtual bool operator==(const Datatype& type) const {
    return GetTypeCategory() == type.GetTypeCategory();
  }

  bool operator!=(const Datatype& type) const {
    return !operator==(type);
  }
};

class U8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class U128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I8Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I16Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class I128Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F32Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class F64Type : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

class BoolType : public Datatype {
 public:
  TypeCategory GetTypeCategory() const override;
  bool IsCastableTo(TypeCategory castType) const override;
  AstNodePtr GetDefaultValue() const override;
};

TypeCategory
TokenTypeToDataType(TokenType tokenType);

bool
IsUnsigned(TypeCategory dataType);

bool
IsNumericType(TypeCategory dataType);

#endif //KODAMA_SRC_ANALYZER_TYPE_DATATYPE_HPP_
