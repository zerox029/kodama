//
// Created by emma on 21/09/23.
//

#include "typeChecker.hpp"
#include "../../errors/errorFactory.hpp"
#include <sstream>

TypeChecker::TypeChecker(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : symbolTable{},
      lastVisitedType{},
      code{code},
      tokens{tokens} {}

std::vector<Errors::Error>
TypeChecker::TypeCheck(const AstNodePtr& ast) {
  ast->Accept(this);

  return errors;
}

void
TypeChecker::Visit(Program* element) {
  for (const AstNodePtr& statement : element->GetStatements()) {
    statement->Accept(this);
  }
}

void
TypeChecker::Visit(Struct* element) {
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
TypeChecker::Visit(Parameter* element) {
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
    errors.push_back(Errors::Generate(Errors::RETURN_TYPE_MISMATCH,
                                      element->GetToken().GetLocation(),
                                      code,
                                      currentScopeType->GetTypeNameString(),
                                      lastVisitedType->GetTypeNameString()));
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
TypeChecker::Visit(ForLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeChecker::Visit(AssignmentExpression* element) {
  TypePtr previousScopeType = currentScopeType;
  currentScopeType = element->GetDataType();

  element->GetValue()->Accept(this);

  // Variable value type mismatch
  if (element->GetDataType()->GetTypeName() != lastVisitedType->GetTypeName()) {
    errors.push_back(Errors::Generate(Errors::VAR_TYPE_MISMATCH,
                                      element->GetToken().GetLocation(),
                                      code,
                                      currentScopeType->GetTypeNameString(),
                                      lastVisitedType->GetTypeNameString()));
  }

  symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
  currentScopeType = previousScopeType;
}

void
TypeChecker::Visit(ReassignmentExpression* element) {
  //TODO
}

void
TypeChecker::Visit(BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  TypePtr lhs = lastVisitedType;
  element->GetRhs()->Accept(this);
  TypePtr rhs = lastVisitedType;

  if (lhs->GetTypeName() != rhs->GetTypeName()) {
    errors.push_back(Errors::Generate(Errors::RETURN_TYPE_MISMATCH,
                                      element->GetToken().GetLocation(),
                                      code,
                                      currentScopeType->GetTypeNameString(),
                                      lastVisitedType->GetTypeNameString()));
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
  if (element->GetDatatype()->GetTypeName() != currentScopeType->GetTypeName()
      && element->GetDatatype()->GetTypeCategory() == currentScopeType->GetTypeCategory()) {
    element->SetDatatype(currentScopeType);
  }

  lastVisitedType = element->GetDatatype();
}

void
TypeChecker::Visit(DecimalLiteral* element) {
  if (element->GetDatatype()->GetTypeName() != currentScopeType->GetTypeName()
      && element->GetDatatype()->GetTypeCategory() == currentScopeType->GetTypeCategory()) {
    element->SetDatatype(currentScopeType);
  }

  lastVisitedType = element->GetDatatype();
}

void
TypeChecker::CheckConditionType(const AstNodePtr& condition) {
  condition->Accept(this);

  if (lastVisitedType->GetTypeCategory() != BOOLEAN) {
    errors.push_back(Errors::Generate(Errors::RETURN_TYPE_MISMATCH,
                                      condition->GetToken().GetLocation(),
                                      code,
                                      std::string("bool"),
                                      lastVisitedType->GetTypeNameString()));
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