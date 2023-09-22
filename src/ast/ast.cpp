//
// Created by emma on 14/09/23.
//

#include "ast.hpp"

#include <utility>

/// Function declaration
FunctionDeclaration::FunctionDeclaration(std::string identifier,
                                         std::vector<AstNodePtr> parameters,
                                         TypePtr returnType,
                                         AstNodePtr body)
    : identifier{std::move(identifier)},
      parameters{std::move(parameters)},
      returnType{std::move(returnType)},
      body{std::move(body)} {}

AstNodePtr
FunctionDeclaration::GetBody() const { return body; }

std::string
FunctionDeclaration::GetIdentifier() const { return identifier; }

std::vector<AstNodePtr>
FunctionDeclaration::GetParameters() const { return parameters; }

TypePtr
FunctionDeclaration::GetReturnType() const { return returnType; }

AstNodeKind
FunctionDeclaration::GetKind() const { return AST_FUNC_DEC; }


/// Function Parameter
FunctionParameter::FunctionParameter(std::string identifier, TypePtr dataType)
    : identifier{std::move(identifier)}, datatype{std::move(dataType)} {}

std::string
FunctionParameter::GetIdentifier() const { return identifier; }

TypePtr
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
AssignmentExpression::AssignmentExpression(std::string identifier, TypePtr dataType, AstNodePtr value)
    : identifier{std::move(identifier)}, dataType(std::move(dataType)), value{std::move(value)} {}

std::string
AssignmentExpression::GetIdentifier() const { return identifier; }

TypePtr
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
NumberLiteral::NumberLiteral(std::string_view integerValue) : integerValue{integerValue} {}

NumberLiteral::NumberLiteral(std::string_view integerValue, std::string_view decimalValue)
    : integerValue{integerValue}, decimalValue{decimalValue} {}

std::string
NumberLiteral::GetIntegerValue() const { return integerValue; }

std::string
NumberLiteral::GetDecimalValue() const { return decimalValue; }

std::string
NumberLiteral::GetValue() const { return integerValue + (decimalValue.empty() ? "" : ("." + decimalValue)); }

AstNodeKind
NumberLiteral::GetKind() const { return AST_NUMBER_LITERAL; }


/// Function call
FunctionCall::FunctionCall(std::string_view identifier, std::vector<AstNodePtr> arguments, bool isExtern)
    : identifier{identifier}, arguments{std::move(arguments)}, isExtern{isExtern} {}

std::string
FunctionCall::GetIdentifier() const { return identifier; }

std::vector<AstNodePtr>
FunctionCall::GetArguments() const { return arguments; }

bool
FunctionCall::IsExtern() const { return isExtern; }

AstNodeKind
FunctionCall::GetKind() const { return AST_FUNCTION_CALL; }


/// Function argument
FunctionArgument::FunctionArgument(std::string_view identifier, AstNodePtr value)
    : identifier{identifier}, value{std::move(value)} {}

std::string
FunctionArgument::GetIdentifier() const { return identifier; }

AstNodePtr
FunctionArgument::GetValue() const { return value; }

AstNodeKind
FunctionArgument::GetKind() const { return AST_FUNCTION_ARGUMENT; }


/// Variable
Variable::Variable(std::string_view identifier) : identifier{identifier} {}

std::string
Variable::GetIdentifier() const { return identifier; }

AstNodeKind
Variable::GetKind() const { return AST_VARIABLE; }


/// String literal
StringLiteral::StringLiteral(std::string_view value) : value{value} {}

std::string
StringLiteral::GetValue() const { return value; }

AstNodeKind
StringLiteral::GetKind() const { return AST_STRING_LITERAL; }


/// Bool value
BoolValue::BoolValue(bool value) : value{value} {}

bool
BoolValue::GetValue() const { return value; }

AstNodeKind
BoolValue::GetKind() const { return AST_BOOL_VALUE; }


/// Null value
AstNodeKind
NullValue::GetKind() const { return AST_NULL_VALUE; }