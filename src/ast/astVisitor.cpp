//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

llvm::Value*
Program::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
Return::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
Assignment::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
NumberLiteral::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
BinaryOperation::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}