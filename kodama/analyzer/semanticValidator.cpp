//
// Created by emma on 23/09/23.
//

#include "semanticValidator.hpp"
#include "../errors/errorFactory.hpp"

SemanticValidator::SemanticValidator(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : code{code}, tokens{tokens} {}

std::vector<Error>
SemanticValidator::Validate(const AstNodePtr& ast) {
  return errors;
}

void
SemanticValidator::Visit(Program* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
SemanticValidator::Visit(FunctionDeclaration* element) {
  element->GetBody()->Accept(this);
}

void
SemanticValidator::Visit(FunctionParameter* element) {

}

void
SemanticValidator::Visit(Block* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
SemanticValidator::Visit(ReturnStatement* element) {

}

void
SemanticValidator::Visit(IfStatement* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(IfElseStatement* element) {
  element->GetConsequent()->Accept(this);
  element->GetAlternative()->Accept(this);
}

void
SemanticValidator::Visit(WhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(DoWhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(AssignmentExpression* element) {
  // Verify that the variable was not already defined
  if (symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ILLEGAL_REDEFINITION,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  } else {
    symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
  }

  element->GetValue()->Accept(this);
}

void
SemanticValidator::Visit(ReassignmentExpression* element) {
  // Verify that the variable being reassigned to exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }

  //Verify that the variable being reassigned to is mutable
  if (!symbolTable.at(element->GetIdentifier())->IsMutable()) {
    errors.push_back(Errors::Generate(Errors::ASSIGN_VAL,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }

  element->GetValue()->Accept(this);
}

void
SemanticValidator::Visit(BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  element->GetRhs()->Accept(this);
}

void
SemanticValidator::Visit(FunctionCall* element) {
  // Verify that the function exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }
}

void
SemanticValidator::Visit(FunctionArgument* element) {

}

void
SemanticValidator::Visit(Variable* element) {
  // Verify that the function exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }
}

void
SemanticValidator::Visit(IntegerLiteral* element) {

}

void
SemanticValidator::Visit(DecimalLiteral* element) {

}

void
SemanticValidator::Visit(StringLiteral* element) {

}

void
SemanticValidator::Visit(BoolValue* element) {

}

void
SemanticValidator::Visit(NullValue* element) {

}