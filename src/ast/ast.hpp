//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include <memory>
#include <llvm/IR/Value.h>

enum AstNodeKind {
  AST_NUMBER_LITERAL,
  AST_BINARY_OPERATION
};

class AstVisitor;

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual Token GetToken() const = 0;
  virtual AstNodeKind GetKind() const = 0;
  virtual std::string Stringify() const = 0;
  virtual llvm::Value* Accept(AstVisitor* visitor) const = 0;
};

class NumberLiteral : public AstNode {
 private:
  Token token;
  std::string value;

 public:
  NumberLiteral(Token tok, std::string_view val);

  std::string GetValue() const;

  Token GetToken() const;
  AstNodeKind GetKind() const;
  std::string Stringify() const;
  llvm::Value* Accept(AstVisitor* visitor) const;
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  std::string value;
  std::shared_ptr<AstNode> lhs;
  std::shared_ptr<AstNode> rhs;

 public:
  BinaryOperation(Token operatorTok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs);

  std::shared_ptr<AstNode> GetLhs() const;
  std::shared_ptr<AstNode> GetRhs() const;

  Token GetToken() const;
  AstNodeKind GetKind() const;
  std::string Stringify() const;
  llvm::Value* Accept(AstVisitor* visitor) const;
};

#endif //KODAMA_SRC_EXPRESSION_H_
