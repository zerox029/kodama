//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include <memory>

enum AstNodeKind {
  AST_EXPRESSION,
  AST_NUMBER_LITERAL,
  AST_BINARY_OPERATION
};

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual Token GetToken() const = 0;
  virtual AstNodeKind GetKind() const = 0;
  virtual std::string Stringify() const = 0;
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
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  std::string value;
  std::shared_ptr<AstNode> lhs;
  std::shared_ptr<AstNode> rhs;

 public:
  BinaryOperation(Token operatorTok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs);

  std::shared_ptr<AstNode> GetLhs();
  std::shared_ptr<AstNode> GetRhs();

  Token GetToken() const;
  AstNodeKind GetKind() const;
  std::string Stringify() const;
};

#endif //KODAMA_SRC_EXPRESSION_H_
