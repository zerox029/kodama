//
// Created by emma on 14/09/23.
//

#include "codegen.hpp"
#include <iostream>
#include <llvm/IR/Verifier.h>

Codegen::Codegen() {
  context = std::make_unique<llvm::LLVMContext>();
  builder = std::make_unique<llvm::IRBuilder<>>(*context);
  module = std::make_unique<llvm::Module>("KodamaGenTest", *context);
}

void
Codegen::Print() {
  module->print(llvm::outs(), nullptr);
}

void
Codegen::Generate(const std::shared_ptr<AstNode>& ast) {
  CreateFunction("main", llvm::FunctionType::get(builder->getInt32Ty(), false));

  builder->CreateRet(ast->Accept(this));
}

llvm::Value*
Codegen::Visit(const NumberLiteral* element) {
  llvm::Value* val = llvm::ConstantInt::get(*context, llvm::APInt(64, element->GetValue(), 10));
  return val;
}

llvm::Value*
Codegen::Visit(const BinaryOperation* element) {
  llvm::Value* lhs = element->GetLhs()->Accept(this);
  llvm::Value* rhs = element->GetRhs()->Accept(this);

  llvm::Value* op = builder->CreateAdd(lhs, rhs, "addtmp");
  return op;
}

// Temporary
llvm::Function* Codegen::CreateFunction(const std::string& fnName, llvm::FunctionType* fnType) {
  llvm::Function* fn = module->getFunction(fnName);

  if(fn == nullptr) {
    fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);
    llvm::verifyFunction(*fn);
  }

  auto entry = llvm::BasicBlock::Create(*context, "entry", fn);
  builder->SetInsertPoint(entry);

  return fn;
}