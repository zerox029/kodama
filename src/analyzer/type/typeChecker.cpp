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

  for (const AstNodePtr& parameter : element->GetParameters()) {
    parameter->Accept(this);
  }

  element->GetBody()->Accept(this);

  // TODO: Allow for return statements to arrive before the last statement
  if (lastVisitedType != element->GetReturnType()) {
    fprintf(stderr, "Type mismatch in function definition\n");
  }
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
    fprintf(stderr, "Type mismatch in variable assignment\n");
  }
}

void
TypeChecker::Visit(const BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  TypePtr lhs = lastVisitedType;
  element->GetRhs()->Accept(this);
  TypePtr rhs = lastVisitedType;

  if (lhs != rhs) {
    std::stringstream errorMessage;
    errorMessage << "expected " << lhs->GetTypeNameString() << " but got " << rhs->GetTypeNameString();
    Location location = element->GetRhs()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage.str(), element->GetRhs()->GetToken().GetLocation(), code.at(location.lineNumber)};
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
TypeChecker::Visit(const NumberLiteral* element) {
  // 64-bit size by default
  if (element->GetDecimalValue().empty()) {
    lastVisitedType = std::make_shared<I64Type>();
  } else {
    lastVisitedType = std::make_shared<F64Type>();
  }
}

void
TypeChecker::CheckConditionType(const AstNodePtr& condition) {
  condition->Accept(this);

  if (lastVisitedType->GetTypeCategory() != BOOLEAN) {
    fprintf(stderr, "Expected bool\n");
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