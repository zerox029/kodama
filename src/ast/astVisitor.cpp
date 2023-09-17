//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

llvm::Value*
Program::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
ReturnStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
IfStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
IfElseStatement::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
WhileLoop::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
DoWhileLoop::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
AssignmentExpression::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
BinaryOperation::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
NumberLiteral::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
Variable::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}