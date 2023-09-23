//
// Created by emma on 21/09/23.
//

#include "typeChecker.hpp"
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
TypeChecker::Visit(FunctionDeclaration* element) {
  symbolTable.insert({element->GetIdentifier(), element->GetReturnType()});
  currentScopeType = element->GetReturnType();

  for (const AstNodePtr& parameter : element->GetParameters()) {
    parameter->Accept(this);
  }

  element->GetBody()->Accept(this);
}

void
TypeChecker::Visit(FunctionParameter* element) {
  symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
}

void
TypeChecker::Visit(Block* element) {
  for (const AstNodePtr& statement : element->GetStatements()) {
    statement->Accept(this);
  }
}

void
TypeChecker::Visit(ReturnStatement* element) {
  element->GetReturnValue()->Accept(this);

  // Return type mismatch
  if (lastVisitedType->GetTypeName() != currentScopeType->GetTypeName()) {
    std::string errorMessage = "expected " + currentScopeType->GetTypeNameString() + " return type but got " + lastVisitedType->GetTypeNameString();
    Location location = element->GetReturnValue()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
}

void
TypeChecker::Visit(IfStatement* element) {
  CheckConditionType(element->GetCondition());
  element->GetConsequent()->Accept(this);
}

void
TypeChecker::Visit(IfElseStatement* element) {
  CheckConditionType(element->GetCondition());
  element->GetConsequent()->Accept(this);
  element->GetAlternative()->Accept(this);
}

void
TypeChecker::Visit(WhileLoop* element) {
  CheckConditionType(element->GetCondition());
  element->GetConsequent()->Accept(this);
}

void
TypeChecker::Visit(DoWhileLoop* element) {
  CheckConditionType(element->GetCondition());
  element->GetConsequent()->Accept(this);
}

void
TypeChecker::Visit(AssignmentExpression* element) {
  TypePtr previousScopeType = currentScopeType;
  currentScopeType = element->GetDataType();

  element->GetValue()->Accept(this);

  // Variable value type mismatch
  if (element->GetDataType()->GetTypeName() != lastVisitedType->GetTypeName()) {
    std::string errorMessage = "expected " + element->GetDataType()->GetTypeNameString() + " but got " + lastVisitedType->GetTypeNameString();
    Location location = element->GetValue()->GetToken().GetLocation();
    Error error{"type mismatch", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }

  symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
  currentScopeType = previousScopeType;
}

void
TypeChecker::Visit(BinaryOperation* element) {
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
TypeChecker::Visit(FunctionCall* element) {
  lastVisitedType = symbolTable.at(element->GetIdentifier());
}

void
TypeChecker::Visit(FunctionArgument* element) {
  element->Accept(this);
}

void
TypeChecker::Visit(Variable* element) {
  lastVisitedType = symbolTable.at(element->GetIdentifier());
}

void
TypeChecker::Visit(IntegerLiteral* element) {
  if(element->GetDatatype()->GetTypeName() != currentScopeType->GetTypeName()
  && element->GetDatatype()->GetTypeCategory() == currentScopeType->GetTypeCategory()) {
    element->SetDatatype(currentScopeType);
  }

  lastVisitedType = element->GetDatatype();
}

void
TypeChecker::Visit(DecimalLiteral* element) {
  if(element->GetDatatype()->GetTypeName() != currentScopeType->GetTypeName()
      && element->GetDatatype()->GetTypeCategory() == currentScopeType->GetTypeCategory()) {
    element->SetDatatype(currentScopeType);
  }

  lastVisitedType = element->GetDatatype();
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
TypeChecker::Visit(StringLiteral* element) {

}

void
TypeChecker::Visit(BoolValue* element) {
  lastVisitedType = std::make_shared<BoolType>();
}

void
TypeChecker::Visit(NullValue* element) {

}