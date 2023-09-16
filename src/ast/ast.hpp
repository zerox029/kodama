//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include "datatypes.hpp"
#include <memory>
#include <llvm/IR/Value.h>

enum AstNodeKind {
  AST_ASSIGNMENT,
  AST_NUMBER_LITERAL,
  AST_BINARY_OPERATION
};

class AstVisitor;

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual AstNodeKind GetKind() const = 0;
  virtual llvm::Value* Accept(AstVisitor* visitor) const = 0;
};

class Assignment : public AstNode {
 private:
  std::string identifier;
  DataType dataType;
  std::shared_ptr<AstNode> value;

 public:
  Assignment(std::string identifier, DataType type, std::shared_ptr<AstNode> value);

  std::string GetIdentifier() const;
  DataType GetDataType() const;
  std::shared_ptr<AstNode> GetValue() const;

  AstNodeKind GetKind() const;
  llvm::Value* Accept(AstVisitor* visitor) const;
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  std::shared_ptr<AstNode> lhs;
  std::shared_ptr<AstNode> rhs;

 public:
  BinaryOperation(Token operatorTok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs);

  std::shared_ptr<AstNode> GetLhs() const;
  std::shared_ptr<AstNode> GetRhs() const;
  Token GetOperator() const;

  AstNodeKind GetKind() const;
  llvm::Value* Accept(AstVisitor* visitor) const;
};

class NumberLiteral : public AstNode {
 private:
  std::string value;

 public:
  NumberLiteral(std::string_view val);

  std::string GetValue() const;

  AstNodeKind GetKind() const;
  llvm::Value* Accept(AstVisitor* visitor) const;
};

#endif //KODAMA_SRC_EXPRESSION_H_
