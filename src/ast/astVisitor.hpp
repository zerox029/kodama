//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
#define KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_

#include "ast.hpp"

class AstVisitor {
 public:
  virtual void Visit(const NumberLiteral* element) = 0;
  virtual void Visit(const BinaryOperation* element) = 0;
};

#endif //KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
