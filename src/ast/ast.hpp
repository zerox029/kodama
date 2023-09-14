//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include <memory>

enum AstNodeKind {
  AST_NUMBER_LITERAL,
  AST_BINARY_OPERATION
};

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual Token getToken() const = 0;
  virtual AstNodeKind getKind() const = 0;
};

class NumberLiteral : public AstNode {
 private:
  Token token;
  std::string value;

 public:
  NumberLiteral(Token tok, std::string_view val);

  std::string getValue() const;

  Token getToken() const;
  AstNodeKind getKind() const;
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  std::shared_ptr<NumberLiteral> lhs;
  std::shared_ptr<NumberLiteral> rhs;

 public:
  BinaryOperation(Token tok, std::shared_ptr<NumberLiteral> lhs, std::shared_ptr<NumberLiteral> rhs);

  std::shared_ptr<NumberLiteral> getLHS();
  std::shared_ptr<NumberLiteral> getRHS();

  Token getToken() const;
  AstNodeKind getKind() const;
};

#endif //KODAMA_SRC_EXPRESSION_H_
