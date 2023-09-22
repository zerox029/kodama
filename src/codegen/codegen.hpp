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
#include <map>

class Codegen : public AstVisitor {
 private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::AllocaInst*> namedValues;

  llvm::Value* lastGeneratedValue;

  bool handlingUnsignedVariable;
  llvm::Type* currentVariableDataType;

  llvm::Type* ResolveLLVMType(TypeCategory type);
  llvm::Function* CreateFunction(const std::string& fnName,
                                 llvm::FunctionType* fnType,
                                 std::vector<AstNodePtr> parameters);
  void setupExternFunctions();

 public:
  Codegen();

  void Print();
  void saveModuleToFile(const std::string& fileName);
  void Generate(const AstNodePtr& ast);

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
  void Visit(const BoolValue* element) override;
  void Visit(const NullValue* element) override;
};

#endif //KODAMA_SRC_CODEGEN_CODEGEN_HPP_
