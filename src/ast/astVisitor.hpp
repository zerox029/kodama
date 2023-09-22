//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
#define KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_

#include "ast.hpp"
#include <llvm/IR/Value.h>

class AstVisitor {
 public:
  virtual void Visit(const FunctionDeclaration* element) = 0;
  virtual void Visit(const FunctionParameter* element) = 0;
  virtual void Visit(const Block* element) = 0;
  virtual void Visit(const ReturnStatement* element) = 0;
  virtual void Visit(const IfStatement* element) = 0;
  virtual void Visit(const IfElseStatement* element) = 0;
  virtual void Visit(const WhileLoop* element) = 0;
  virtual void Visit(const DoWhileLoop* element) = 0;
  virtual void Visit(const AssignmentExpression* element) = 0;
  virtual void Visit(const BinaryOperation* element) = 0;
  virtual void Visit(const FunctionCall* element) = 0;
  virtual void Visit(const FunctionArgument* element) = 0;
  virtual void Visit(const Variable* element) = 0;
  virtual void Visit(const NumberLiteral* element) = 0;
  virtual void Visit(const StringLiteral* element) = 0;
  virtual void Visit(const BoolValue* element) = 0;
  virtual void Visit(const NullValue* element) = 0;
};

#endif //KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
