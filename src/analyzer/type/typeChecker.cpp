//
// Created by emma on 21/09/23.
//

#include "typeChecker.hpp"
#include "../../errors/error.hpp"
#include <sstream>

TypeChecker::TypeChecker(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : symbolTable{},
      lastVisitedType{},
      code{code},
      tokens{tokens} {}

std::vector<Error>
TypeChecker::TypeCheck(const AstNodePtr& ast) {
  ast->Accept(this);

  return errors;
}

void
TypeChecker::Visit(const FunctionDeclaration* element) {
  symbolTable.insert({element->GetIdentifier(), element->GetReturnType()});
  currentScopeName = element->GetIdentifier();

  for (const AstNodePtr& parameter : element->GetParameters()) {
    parameter->Accept(this);
  }

  element->GetBody()->Accept(this);
}

void
TypeChecker::Visit(const FunctionParameter* element) {
  symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
}

void
TypeChecker::Visit(const Block* element) {
  for (const AstNodePtr& statement : element->GetStatements()) {
    statement->Accept(this);
  }
}

void
TypeChecker::Visit(const ReturnStatement* element) {
  element->GetReturnValue()->Accept(this);

  TypePtr currentReturnType = symbolTable.at(currentScopeName);
  if (lastVisitedType != currentReturnType) {
    std::string errorMessage = "expected " + currentReturnType->GetTypeNameString() + " because of return type but got " + lastVisitedType->GetTypeNameString();
    Location location = element->GetReturnValue()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
}

void
TypeChecker::Visit(const IfStatement* element) {
  CheckConditionType(element->GetCondition());
}

void
TypeChecker::Visit(const IfElseStatement* element) {
  CheckConditionType(element->GetCondition());
}

void
TypeChecker::Visit(const WhileLoop* element) {
  CheckConditionType(element->GetCondition());
}

void
TypeChecker::Visit(const DoWhileLoop* element) {
  CheckConditionType(element->GetCondition());
}

void
TypeChecker::Visit(const AssignmentExpression* element) {
  element->GetValue()->Accept(this);

  symbolTable.insert({element->GetIdentifier(), element->GetDataType()});

  if (element->GetDataType()->GetTypeCategory() != lastVisitedType->GetTypeCategory()) {
    std::string errorMessage = "expected " + element->GetDataType()->GetTypeNameString() + " but got " + lastVisitedType->GetTypeNameString();
    Location location = element->GetValue()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
}

void
TypeChecker::Visit(const BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  TypePtr lhs = lastVisitedType;
  element->GetRhs()->Accept(this);
  TypePtr rhs = lastVisitedType;

  if (lhs != rhs) {
    std::string errorMessage = "expected " + lhs->GetTypeNameString() + " but got " + rhs->GetTypeNameString();
    Location location = element->GetRhs()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
}

void
TypeChecker::Visit(const FunctionCall* element) {
  lastVisitedType = symbolTable.at(element->GetIdentifier());
}

void
TypeChecker::Visit(const FunctionArgument* element) {
  element->Accept(this);
}

void
TypeChecker::Visit(const Variable* element) {
  lastVisitedType = symbolTable.at(element->GetIdentifier());
}

void
TypeChecker::Visit(const IntegerLiteral* element) {
  lastVisitedType = std::make_shared<I64Type>();
}

void
TypeChecker::Visit(const DecimalLiteral* element) {
  lastVisitedType = std::make_shared<F64Type>();
}

void
TypeChecker::CheckConditionType(const AstNodePtr& condition) {
  condition->Accept(this);
  if (lastVisitedType->GetTypeCategory() != BOOLEAN) {
    std::string errorMessage = "expected bool but got " + lastVisitedType->GetTypeNameString();
    Location location = condition->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
}

void
TypeChecker::Visit(const StringLiteral* element) {

}

void
TypeChecker::Visit(const BoolValue* element) {
  lastVisitedType = std::make_shared<BoolType>();
}

void
TypeChecker::Visit(const NullValue* element) {

}