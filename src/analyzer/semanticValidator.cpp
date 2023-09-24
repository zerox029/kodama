//
// Created by emma on 23/09/23.
//

#include "semanticValidator.hpp"

SemanticValidator::SemanticValidator(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : code{code}, tokens{tokens} {}

std::vector<Error>
SemanticValidator::Validate(const AstNodePtr& ast) {
  ast->Accept(this);

  return errors;
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
  for(const AstNodePtr& node : element->GetStatements()) {
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
  if(symbolTable.contains(element->GetIdentifier())) {
    std::string errorMessage = "illegal redefinition of '" + element->GetIdentifier() + "'";
    Location location = element->GetToken().GetLocation();
    Error error{"semantic error", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
  }
  else {
    symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
  }

  element->GetValue()->Accept(this);
}

void
SemanticValidator::Visit(ReassignmentExpression* element) {
  // Verify that the variable being reassigned to exists
  if(!symbolTable.contains(element->GetIdentifier())) {
    std::string errorMessage = "cannot find symbol '" + element->GetIdentifier() + "' in scope";
    Location location = element->GetToken().GetLocation();
    Error error{"error", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);

    return;
  }

  //Verify that the variable being reassigned to is mutable
  if(!symbolTable.at(element->GetIdentifier())->IsMutable()) {
    std::string errorMessage = "cannot assign twice to value '" + element->GetIdentifier() + "'";
    Location location = element->GetToken().GetLocation();
    Error error{"error", errorMessage, location, code.at(location.lineNumber)};
    errors.push_back(error);
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

}

void
SemanticValidator::Visit(FunctionArgument* element) {

}

void
SemanticValidator::Visit(Variable* element) {

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