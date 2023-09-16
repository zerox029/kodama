//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Assignment
Assignment::Assignment(std::string  identifier, DataType dataType, std::shared_ptr<AstNode> value)
  : identifier{std::move(identifier)}, dataType(dataType), value{std::move(value)} {}

std::string
Assignment::GetIdentifier() const { return identifier; }

DataType
Assignment::GetDataType() const { return dataType; }

std::shared_ptr<AstNode>
Assignment::GetValue() const { return value; }

AstNodeKind
Assignment::GetKind() const { return AST_ASSIGNMENT; }


/// Binary Operation
BinaryOperation::BinaryOperation(Token tok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs)
    : operatorToken{tok}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

std::shared_ptr<AstNode>
BinaryOperation::GetLhs() const { return lhs; }

std::shared_ptr<AstNode>
BinaryOperation::GetRhs() const { return rhs; }

Token
BinaryOperation::GetOperator() const { return operatorToken; }

AstNodeKind
BinaryOperation::GetKind() const { return AST_BINARY_OPERATION; }

/// Number Literal
NumberLiteral::NumberLiteral(std::string_view val) : value{val} {}

std::string
NumberLiteral::GetValue() const { return value; }

AstNodeKind
NumberLiteral::GetKind() const { return AST_NUMBER_LITERAL; }