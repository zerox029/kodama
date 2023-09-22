//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

void
FunctionDeclaration::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
FunctionParameter::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
Block::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
ReturnStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
IfStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
IfElseStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
WhileLoop::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
DoWhileLoop::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
AssignmentExpression::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
BinaryOperation::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
FunctionCall::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
FunctionArgument::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
Variable::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
NumberLiteral::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
StringLiteral::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

void
NullValue::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}