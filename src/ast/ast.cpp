//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Number Literal
NumberLiteral::NumberLiteral(Token tok, std::string_view val) : token{tok}, value{val} {}

std::string
NumberLiteral::GetValue() const { return value; }

Token
NumberLiteral::GetToken() const { return token; }

AstNodeKind
NumberLiteral::GetKind() const { return AST_NUMBER_LITERAL; }

std::string
NumberLiteral::Stringify() const {
  std::string s = "dede";
  return value;
}

/// Binary Operation
BinaryOperation::BinaryOperation(Token tok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs)
    : operatorToken{tok}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

std::shared_ptr<AstNode>
BinaryOperation::GetLhs() const { return lhs; }

std::shared_ptr<AstNode>
BinaryOperation::GetRhs() const { return rhs; }

Token
BinaryOperation::GetToken() const { return operatorToken; }

AstNodeKind
BinaryOperation::GetKind() const { return AST_BINARY_OPERATION; }

std::string
BinaryOperation::Stringify() const {
  std::string op = std::string{operatorToken.getStr()};
  return "[" + lhs->Stringify() + " " + op + " " + rhs->Stringify() + "]";
}

/// Assignment
Assignment::Assignment(Token identifierToken, Token typeToken, std::shared_ptr<AstNode> value)
    : identifier{identifierToken.getStr()}, type(typeToken), value{value} {}

std::string
Assignment::GetIdentifier() const { return identifier; }

Token
Assignment::GetType() const { return type; }

std::shared_ptr<AstNode>
Assignment::GetValue() const { return value; }

Token
Assignment::GetToken() const { return type; }

AstNodeKind
Assignment::GetKind() const { return AST_ASSIGNMENT; }

std::string
Assignment::Stringify() const {
  return "[Assignment]";
}