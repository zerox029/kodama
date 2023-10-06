//
// Created by emma on 23/09/23.
//

#include "typeInference.hpp"

void
TypeInference::Visit(Program* element) {
  for(const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
TypeInference::Visit(Struct* element) {
}


void
TypeInference::Visit(FunctionDeclaration* element) {
  element->GetBody()->Accept(this);
}

void
TypeInference::Visit(Parameter* element) {

}

void
TypeInference::Visit(Block* element) {
  for(const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
TypeInference::Visit(ReturnStatement* element) {

}

void
TypeInference::Visit(IfStatement* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(IfElseStatement* element) {
  element->GetConsequent()->Accept(this);
  element->GetAlternative()->Accept(this);
}

void
TypeInference::Visit(WhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(DoWhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
TypeInference::Visit(ForLoop* element) {
  element->GetConsequent()->Accept(this);
}


void
TypeInference::Visit(AssignmentExpression* element) {
  currentScopeType = element->GetDataType();
  element->GetValue()->Accept(this);
}

void
TypeInference::Visit(ReassignmentExpression* element) {
  element->GetValue()->Accept(this);
}

void
TypeInference::Visit(BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  element->GetRhs()->Accept(this);
}

void
TypeInference::Visit(FunctionCall* element) {

}

void
TypeInference::Visit(FunctionArgument* element) {

}

void
TypeInference::Visit(Variable* element) {

}

void
TypeInference::Visit(IntegerLiteral* element) {

}

void
TypeInference::Visit(DecimalLiteral* element) {

}

void
TypeInference::Visit(StringLiteral* element) {

}

void
TypeInference::Visit(BoolValue* element) {

}

void
TypeInference::Visit(NullValue* element) {

}