//
// Created by emma on 23/09/23.
//

#ifndef KODAMA_SRC_ANALYZER_SEMANTICVALIDATOR_HPP_
#define KODAMA_SRC_ANALYZER_SEMANTICVALIDATOR_HPP_

#include "../ast/astVisitor.hpp"
#include "../errors/error.hpp"

class SemanticValidator : public AstVisitor {
 public:
  SemanticValidator(const std::vector<std::string>& code, const std::vector<Token>& tokens);
  std::vector<Errors::Error> Validate(const AstNodePtr& ast);

 private:
  std::vector<std::string> code;
  std::vector<Token> tokens;

  std::unordered_map<std::string, TypePtr> symbolTable;

  std::vector<Errors::Error> errors;

  void Visit(Program* element) override;
  void Visit(Struct* element) override;
  void Visit(FunctionDeclaration* element) override;
  void Visit(Parameter* element) override;
  void Visit(Block* element) override;
  void Visit(ReturnStatement* element) override;
  void Visit(IfStatement* element) override;
  void Visit(IfElseStatement* element) override;
  void Visit(WhileLoop* element) override;
  void Visit(DoWhileLoop* element) override;
  void Visit(ForLoop* element) override;
  void Visit(AssignmentExpression* element) override;
  void Visit(ReassignmentExpression* element) override;
  void Visit(BinaryOperation* element) override;
  void Visit(FunctionCall* element) override;
  void Visit(Argument* element) override;
  void Visit(Variable* element) override;
  void Visit(IntegerLiteral* element) override;
  void Visit(DecimalLiteral* element) override;
  void Visit(StringLiteral* element) override;
  void Visit(BoolValue* element) override;
  void Visit(NullValue* element) override;

  void ValidateFunctionArguments(const std::vector<AstNodePtr>& arguments);
};


#endif //KODAMA_SRC_ANALYZER_SEMANTICVALIDATOR_HPP_
