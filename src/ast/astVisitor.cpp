//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

llvm::Value*
NumberLiteral::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}

llvm::Value*
BinaryOperation::Accept(AstVisitor* visitor) const {
  return visitor->Visit(this);
}