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

  bool handlingUnsignedVariable;

  llvm::Type* ResolveType(DataType type);
  llvm::Function* CreateFunction(const std::string& fnName, llvm::FunctionType* fnType);

 public:
  Codegen();

  void Print();
  void saveModuleToFile(const std::string& fileName);
  void Generate(const AstNodePtr& ast);

  llvm::Value* Visit(const FunctionDeclaration* element) override;
  llvm::Value* Visit(const Block* element) override;
  llvm::Value* Visit(const ReturnStatement* element) override;
  llvm::Value* Visit(const IfStatement* element) override;
  llvm::Value* Visit(const IfElseStatement* element) override;
  llvm::Value* Visit(const WhileLoop* element) override;
  llvm::Value* Visit(const DoWhileLoop* element) override;
  llvm::Value* Visit(const AssignmentExpression* element) override;
  llvm::Value* Visit(const BinaryOperation* element) override;
  llvm::Value* Visit(const NumberLiteral* element) override;
  llvm::Value* Visit(const Variable* element) override;
};

#endif //KODAMA_SRC_CODEGEN_CODEGEN_HPP_
