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


/// ReturnStatement
ReturnStatement::ReturnStatement(std::shared_ptr<AstNode> returnValue) : returnValue{returnValue} {}

std::shared_ptr<AstNode>
ReturnStatement::GetReturnValue() const { return returnValue; }

AstNodeKind
ReturnStatement::GetKind() const { return AST_RETURN; }


/// If Statement
IfStatement::IfStatement(std::shared_ptr<AstNode> condition,
                         std::shared_ptr<AstNode> consequent) : condition{condition}, consequent{consequent} {}

std::shared_ptr<AstNode>
IfStatement::GetCondition() const { return condition; }

std::shared_ptr<AstNode>
IfStatement::GetConsequent() const { return consequent; }

AstNodeKind
IfStatement::GetKind() const { return AST_IF; }


/// IfElse Statement
IfElseStatement::IfElseStatement(std::shared_ptr<AstNode> condition,
                                 std::shared_ptr<AstNode> consequent,
                                 std::shared_ptr<AstNode> alternative)
    : condition{condition}, consequent{consequent}, alternative{alternative} {}

std::shared_ptr<AstNode>
IfElseStatement::GetCondition() const { return condition; }

std::shared_ptr<AstNode>
IfElseStatement::GetConsequent() const { return consequent; }

std::shared_ptr<AstNode>
IfElseStatement::GetAlternative() const { return alternative; }

AstNodeKind
IfElseStatement::GetKind() const { return AST_IFELSE; }


/// AssignmentExpression
AssignmentExpression::AssignmentExpression(std::string identifier, DataType dataType, std::shared_ptr<AstNode> value)
    : identifier{std::move(identifier)}, dataType(dataType), value{std::move(value)} {}

std::string
AssignmentExpression::GetIdentifier() const { return identifier; }

DataType
AssignmentExpression::GetDataType() const { return dataType; }

std::shared_ptr<AstNode>
AssignmentExpression::GetValue() const { return value; }

AstNodeKind
AssignmentExpression::GetKind() const { return AST_ASSIGNMENT; }


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