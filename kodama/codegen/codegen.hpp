//
// Created by emma on 14/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_CODEGEN_HPP_
#define KODAMA_SRC_CODEGEN_CODEGEN_HPP_

#include "../ast/astVisitor.hpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <map>

class Codegen : public AstVisitor {
 private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::AllocaInst*> namedValues;

  std::unique_ptr<llvm::legacy::FunctionPassManager> functionPassManager;

  llvm::Value* lastGeneratedValue;

  TypePtr currentFunctionType;
  std::string currentFunctionName;
  TypePtr currentVariableType;

  bool skipOptimizations;

  llvm::Function* CreateFunction(const std::string& fnName,
                                 llvm::FunctionType* fnType,
                                 std::vector<AstNodePtr> parameters);
  void setupExternFunctions();

 public:
  Codegen(const bool skipOptimizations);

  void Print();
  void EmitLLVMIR(const std::string& fileName);
  void EmitObjectCode(const std::string& fileName);
  void Generate(const AstNodePtr& ast);

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
  void Visit(FunctionArgument* element) override;
  void Visit(Variable* element) override;
  void Visit(IntegerLiteral* element) override;
  void Visit(DecimalLiteral* element) override;
  void Visit(StringLiteral* element) override;
  void Visit(BoolValue* element) override;
  void Visit(NullValue* element) override;
};

#endif //KODAMA_SRC_CODEGEN_CODEGEN_HPP_
