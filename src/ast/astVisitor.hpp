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
  virtual llvm::Value* Visit(const ReturnStatement* element) = 0;
  virtual llvm::Value* Visit(const IfStatement* element) = 0;
  virtual llvm::Value* Visit(const IfElseStatement* element) = 0;
  virtual llvm::Value* Visit(const AssignmentExpression* element) = 0;
  virtual llvm::Value* Visit(const BinaryOperation* element) = 0;
  virtual llvm::Value* Visit(const NumberLiteral* element) = 0;
  virtual llvm::Value* Visit(const Variable* element) = 0;
};

#endif //KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
