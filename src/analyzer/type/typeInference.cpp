//
// Created by emma on 23/09/23.
//

#include "typeInference.hpp"

void
TypeInference::Visit(const FunctionDeclaration* element) {
  element->GetBody()->Accept(this);
}

void
TypeInference::Visit(const FunctionParameter* element) {

}

void
TypeInference::Visit(const Block* element) {
  for(const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
TypeInference::Visit(const ReturnStatement* element) {

}

void
TypeInference::Visit(const IfStatement* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(const IfElseStatement* element) {
  element->GetConsequent()->Accept(this);
  element->GetAlternative()->Accept(this);
}

void
TypeInference::Visit(const WhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(const DoWhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(const AssignmentExpression* element) {
  currentScopeType = element->GetDataType();
  element->GetValue()->Accept(this);
}

void
TypeInference::Visit(const BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  element->GetRhs()->Accept(this);
}

void
TypeInference::Visit(const FunctionCall* element) {

}

void
TypeInference::Visit(const FunctionArgument* element) {

}

void
TypeInference::Visit(const Variable* element) {

}

void
TypeInference::Visit(const IntegerLiteral* element) {

}

void
TypeInference::Visit(const DecimalLiteral* element) {

}

void
TypeInference::Visit(const StringLiteral* element) {

}

void
TypeInference::Visit(const BoolValue* element) {

}

void
TypeInference::Visit(const NullValue* element) {

}