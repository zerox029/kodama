//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Function declaration
FunctionDeclaration::FunctionDeclaration(std::string identifier,
                                         std::vector<AstNodePtr> parameters,
                                         DataType returnType,
                                         AstNodePtr body)
    : identifier{std::move(identifier)},
      parameters{std::move(parameters)},
      returnType{returnType},
      body{std::move(body)} {}

AstNodePtr
FunctionDeclaration::GetBody() const { return body; }

std::string
FunctionDeclaration::GetIdentifier() const { return identifier; }

std::vector<AstNodePtr>
FunctionDeclaration::GetParameters() const { return parameters; }

DataType
FunctionDeclaration::GetReturnType() const { return returnType; }

AstNodeKind
FunctionDeclaration::GetKind() const { return AST_FUNC_DEC; }


/// Function Parameter
FunctionParameter::FunctionParameter(std::string identifier, DataType dataType)
    : identifier{std::move(identifier)}, datatype{dataType} {}

std::string
FunctionParameter::GetIdentifier() const { return identifier; }

DataType
FunctionParameter::GetDataType() const { return datatype; }

AstNodeKind
FunctionParameter::GetKind() const { return AST_FUNC_PARAM; }


/// Block
Block::Block(std::vector<AstNodePtr> statements) : statements{std::move(statements)} {}

std::vector<AstNodePtr>
Block::GetStatements() const { return statements; }

AstNodeKind
Block::GetKind() const { return AST_BLOCK; }


/// ReturnStatement
ReturnStatement::ReturnStatement(AstNodePtr returnValue) : returnValue{std::move(returnValue)} {}

AstNodePtr
ReturnStatement::GetReturnValue() const { return returnValue; }

AstNodeKind
ReturnStatement::GetKind() const { return AST_RETURN; }


/// If Statement
IfStatement::IfStatement(AstNodePtr condition, AstNodePtr consequent)
    : condition{std::move(condition)}, consequent{std::move(consequent)} {}

AstNodePtr
IfStatement::GetCondition() const { return condition; }

AstNodePtr
IfStatement::GetConsequent() const { return consequent; }

AstNodeKind
IfStatement::GetKind() const { return AST_IF; }


/// IfElse Statement
IfElseStatement::IfElseStatement(AstNodePtr condition,
                                 AstNodePtr consequent,
                                 AstNodePtr alternative)
    : condition{std::move(condition)}, consequent{std::move(consequent)}, alternative{std::move(alternative)} {}

AstNodePtr
IfElseStatement::GetCondition() const { return condition; }

AstNodePtr
IfElseStatement::GetConsequent() const { return consequent; }

AstNodePtr
IfElseStatement::GetAlternative() const { return alternative; }

AstNodeKind
IfElseStatement::GetKind() const { return AST_IF_ELSE; }


/// While Loop
WhileLoop::WhileLoop(AstNodePtr condition, AstNodePtr consequent)
    : condition{std::move(condition)}, consequent{std::move(consequent)} {}

AstNodePtr
WhileLoop::GetCondition() const { return condition; }

AstNodePtr
WhileLoop::GetConsequent() const { return consequent; }

AstNodeKind
WhileLoop::GetKind() const { return AST_WHILE; }


/// Do While Loop
DoWhileLoop::DoWhileLoop(AstNodePtr condition, AstNodePtr consequent)
    : condition{std::move(condition)}, consequent{std::move(consequent)} {}

AstNodePtr
DoWhileLoop::GetCondition() const { return condition; }

AstNodePtr
DoWhileLoop::GetConsequent() const { return consequent; }

AstNodeKind
DoWhileLoop::GetKind() const { return AST_DO_WHILE; }


/// AssignmentExpression
AssignmentExpression::AssignmentExpression(std::string identifier, DataType dataType, AstNodePtr value)
    : identifier{std::move(identifier)}, dataType(dataType), value{std::move(value)} {}

std::string
AssignmentExpression::GetIdentifier() const { return identifier; }

DataType
AssignmentExpression::GetDataType() const { return dataType; }

AstNodePtr
AssignmentExpression::GetValue() const { return value; }

AstNodeKind
AssignmentExpression::GetKind() const { return AST_ASSIGNMENT; }


/// Binary Operation
BinaryOperation::BinaryOperation(Token tok, AstNodePtr lhs, AstNodePtr rhs)
    : operatorToken{std::move(tok)}, lhs{std::move(lhs)}, rhs{std::move(rhs)} {}

AstNodePtr
BinaryOperation::GetLhs() const { return lhs; }

AstNodePtr
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