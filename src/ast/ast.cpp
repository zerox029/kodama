//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Program
Program::Program(std::vector<std::shared_ptr<AstNode>> statements) : statements{statements} {}

std::vector<std::shared_ptr<AstNode>>
Program::GetStatements() const { return statements; }

AstNodeKind
Program::GetKind() const { return AST_PROGRAM; }


/// Return
Return::Return(std::shared_ptr<AstNode> returnValue) : returnValue{returnValue} {}

std::shared_ptr<AstNode>
Return::GetReturnValue() const { return returnValue; }

AstNodeKind
Return::GetKind() const { return AST_RETURN; }

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
    : operatorToken{std::move(tok)}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

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


/// Variable
Variable::Variable(std::string_view identifier) : identifier{identifier} {}

std::string
Variable::GetIdentifier() const { return identifier; }

AstNodeKind
Variable::GetKind() const { return AST_VARIABLE; }