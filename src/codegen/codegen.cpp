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
  handlingUnsignedVariable = false;
}

void
Codegen::Print() {
  module->print(llvm::outs(), nullptr);
}

void
Codegen::Generate(const std::shared_ptr<AstNode>& ast) {
  llvm::Function* fn = CreateFunction("main", llvm::FunctionType::get(builder->getInt64Ty(), false));

  ast->Accept(this);
}

llvm::Value*
Codegen::Visit(const Assignment* element) {
  handlingUnsignedVariable = IsUnsigned(element->GetDataType());

  llvm::Type* varType = ResolveType(element->GetDataType());
  llvm::AllocaInst* variableAllocation = builder->CreateAlloca(varType, nullptr, element->GetIdentifier());

  llvm::Value* initialValue = element->GetValue()->Accept(this);
  initialValue->mutateType(varType);

  builder->CreateStore(initialValue, variableAllocation);

  handlingUnsignedVariable = false;

  return nullptr;
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

  switch (element->GetOperator().getTokenType()) {
    case TK_PLUS: // Addition
      return builder->CreateAdd(lhs, rhs, "addtmp");
    case TK_MINUS: // Substraction
      return builder->CreateSub(lhs, rhs, "subtmp");
    case TK_STAR: // Multiplication
      return builder->CreateMul(lhs, rhs, "multmp");
    case TK_SLASH: { // Division
      if(handlingUnsignedVariable){
        return builder->CreateUDiv(lhs, rhs, "divtmp");
      }
      else {
        return builder->CreateSDiv(lhs, rhs, "divtmp");
      }
    }
    case TK_PERCENT: { // Modulo
      if(handlingUnsignedVariable){
        return builder->CreateURem(lhs, rhs, "modtmp");
      }
      else {
        return builder->CreateSRem(lhs, rhs, "modtmp");
      }
    }
    default:
      return nullptr;
  }
}

llvm::Type*
Codegen::ResolveType(const DataType type) {
  switch (type) {
    case U8:
      return llvm::Type::getInt8Ty(*context);
    case U16:
      return llvm::Type::getInt16Ty(*context);
    case U32:
      return llvm::Type::getInt32Ty(*context);
    case U64:
      return llvm::Type::getInt64Ty(*context);
    case U128:
      return llvm::Type::getInt128Ty(*context);
    case I8:
      return llvm::Type::getInt8Ty(*context);
    case I16:
      return llvm::Type::getInt16Ty(*context);
    case I32:
      return llvm::Type::getInt32Ty(*context);
    case I64:
      return llvm::Type::getInt64Ty(*context);
    case I128:
      return llvm::Type::getInt128Ty(*context);
    default:
      return nullptr;
  }
}

// Temporary
llvm::Function*
Codegen::CreateFunction(const std::string& fnName, llvm::FunctionType* fnType) {
  llvm::Function* fn = module->getFunction(fnName);

  if (fn == nullptr) {
    fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);
    llvm::verifyFunction(*fn);
  }

  auto entry = llvm::BasicBlock::Create(*context, "entry", fn);
  builder->SetInsertPoint(entry);

  return fn;
}