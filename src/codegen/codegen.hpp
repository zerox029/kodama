//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_CODEGEN_HPP_
#define KODAMA_SRC_CODEGEN_CODEGEN_HPP_

#include "../ast/astVisitor.hpp"

class Codegen : public AstVisitor {
  void Visit(const NumberLiteral* element);
  void Visit(const BinaryOperation* element);
};

#endif //KODAMA_SRC_CODEGEN_CODEGEN_HPP_
