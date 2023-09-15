//
// Created by emma on 14/09/23.
//

#include "astVisitor.hpp"

void
NumberLiteral::Accept(AstVisitor* visitor) const {
  visitor->Visit(this);
}

void
BinaryOperation::Accept(AstVisitor* visitor) const {
  visitor->Visit(this);
}