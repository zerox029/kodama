//
// Created by emma on 21/09/23.
//

#ifndef KODAMA_SRC_ANALYZER_TYPECHECKER_HPP_
#define KODAMA_SRC_ANALYZER_TYPECHECKER_HPP_

#include "../../ast/astVisitor.hpp"

class TypeChecker : public AstVisitor {
  void Visit(const FunctionDeclaration* element) override;
  void Visit(const FunctionParameter* element) override;
  void Visit(const Block* element) override;
  void Visit(const ReturnStatement* element) override;
  void Visit(const IfStatement* element) override;
  void Visit(const IfElseStatement* element) override;
  void Visit(const WhileLoop* element) override;
  void Visit(const DoWhileLoop* element) override;
  void Visit(const AssignmentExpression* element) override;
  void Visit(const BinaryOperation* element) override;
  void Visit(const FunctionCall* element) override;
  void Visit(const FunctionArgument* element) override;
  void Visit(const Variable* element) override;
  void Visit(const NumberLiteral* element) override;
  void Visit(const StringLiteral* element) override;
  void Visit(const NullValue* element) override;
};


#endif //KODAMA_SRC_ANALYZER_TYPECHECKER_HPP_