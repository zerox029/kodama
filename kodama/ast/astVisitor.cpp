//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

void
Program::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
Struct::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
FunctionDeclaration::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
Parameter::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
Block::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
ReturnStatement::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
IfStatement::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
IfElseStatement::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
WhileLoop::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
DoWhileLoop::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
ForLoop::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
AssignmentExpression::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
ReassignmentExpression::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
BinaryOperation::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
FunctionCall::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
FunctionArgument::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
Variable::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
IntegerLiteral::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
DecimalLiteral::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
StringLiteral::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
BoolValue::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}

void
NullValue::Accept(AstVisitor* visitor) {
  return visitor->Visit(this);
}