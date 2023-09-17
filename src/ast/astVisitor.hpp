//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
#define KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_

#include "ast.hpp"
#include <llvm/IR/Value.h>

class AstVisitor {
 public:
  virtual llvm::Value* Visit(const Program* element) = 0;
  virtual llvm::Value* Visit(const Return* element) = 0;
  virtual llvm::Value* Visit(const Assignment* element) = 0;
  virtual llvm::Value* Visit(const NumberLiteral* element) = 0;
  virtual llvm::Value* Visit(const BinaryOperation* element) = 0;
};

#endif //KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
