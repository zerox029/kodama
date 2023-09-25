//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
#define KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_

#include "ast.hpp"
#include <llvm/IR/Value.h>

class AstVisitor {
 public:
  virtual void Visit(FunctionDeclaration* element) = 0;
  virtual void Visit(FunctionParameter* element) = 0;
  virtual void Visit(Block* element) = 0;
  virtual void Visit(ReturnStatement* element) = 0;
  virtual void Visit(IfStatement* element) = 0;
  virtual void Visit(IfElseStatement* element) = 0;
  virtual void Visit(WhileLoop* element) = 0;
  virtual void Visit(DoWhileLoop* element) = 0;
  virtual void Visit(AssignmentExpression* element) = 0;
  virtual void Visit(ReassignmentExpression* element) = 0;
  virtual void Visit(BinaryOperation* element) = 0;
  virtual void Visit(FunctionCall* element) = 0;
  virtual void Visit(FunctionArgument* element) = 0;
  virtual void Visit(Variable* element) = 0;
  virtual void Visit(IntegerLiteral* element) = 0;
  virtual void Visit(DecimalLiteral* element) = 0;
  virtual void Visit(StringLiteral* element) = 0;
  virtual void Visit(BoolValue* element) = 0;
  virtual void Visit(NullValue* element) = 0;
};

#endif //KODAMA_SRC_CODEGEN_ASTVISITOR_HPP_
