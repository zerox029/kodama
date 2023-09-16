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
  std::map<std::string, llvm::Value *> namedValues;

  llvm::Function* CreateFunction(const std::string& fnName, llvm::FunctionType* fnType);

 public:
  Codegen();

  void Print();
  void Generate(const std::shared_ptr<AstNode>& ast);

  llvm::Value* Visit(const NumberLiteral* element);
  llvm::Value* Visit(const BinaryOperation* element);
};

#endif //KODAMA_SRC_CODEGEN_CODEGEN_HPP_
