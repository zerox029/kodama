//
// Created by emma on 23/09/23.
//

#include "semanticValidator.hpp"

SemanticValidator::SemanticValidator(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : code{code}, tokens{tokens} {}

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