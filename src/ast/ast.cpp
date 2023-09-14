//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Number Literal

NumberLiteral::NumberLiteral(Token tok, std::string_view val) : token{tok}, value{val} {}

std::string
NumberLiteral::getValue() const { return value; }

Token
NumberLiteral::getToken() const { return token; }

AstNodeKind
NumberLiteral::getKind() const { return AST_NUMBER_LITERAL; }


/// Binary Operation

BinaryOperation::BinaryOperation(Token tok, std::shared_ptr<NumberLiteral> lhs, std::shared_ptr<NumberLiteral> rhs)
  : operatorToken{tok}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

std::shared_ptr<NumberLiteral>
BinaryOperation::getLHS() { return lhs; }

std::shared_ptr<NumberLiteral>
BinaryOperation::getRHS() { return rhs; }

Token
BinaryOperation::getToken() const { return operatorToken; }

AstNodeKind
BinaryOperation::getKind() const { return AST_BINARY_OPERATION; }