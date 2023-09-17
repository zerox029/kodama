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
Codegen::Visit(const Block* element) {
  for (const std::shared_ptr<AstNode>& node : element->GetStatements()) {
    node->Accept(this);
  }

  return nullptr;
}

llvm::Value*
Codegen::Visit(const ReturnStatement* element) {
  return builder->CreateRet(element->GetReturnValue()->Accept(this));
}

llvm::Value*
Codegen::Visit(const IfStatement* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  llvm::Value* ifStatement = builder->CreateCondBr(element->GetCondition()->Accept(this),
                                                   consequent,
                                                   alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(alternative);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  return ifStatement;
}

llvm::Value*
Codegen::Visit(const IfElseStatement* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);
  llvm::BasicBlock* merge = llvm::BasicBlock::Create(*context, "merge", fn);

  llvm::Value* ifStatement = builder->CreateCondBr(element->GetCondition()->Accept(this),
                                                   consequent,
                                                   alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(merge);

  // Generate the alternative
  builder->SetInsertPoint(alternative);
  element->GetAlternative()->Accept(this);
  builder->CreateBr(merge);

  // Generate the merge block
  builder->SetInsertPoint(merge);

  return ifStatement;
}

llvm::Value*
Codegen::Visit(const WhileLoop* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* condition = llvm::BasicBlock::Create(*context, "condition", fn);
  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  // Generate the condition
  builder->CreateBr(condition);
  builder->SetInsertPoint(condition);
  llvm::Value* ifStatement = builder->CreateCondBr(element->GetCondition()->Accept(this),
                                                   consequent,
                                                   alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(condition);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  return ifStatement;
}

llvm::Value*
Codegen::Visit(const DoWhileLoop* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* condition = llvm::BasicBlock::Create(*context, "condition", fn);
  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  // Generate the condition
  builder->CreateBr(consequent);
  builder->SetInsertPoint(condition);
  llvm::Value* ifStatement = builder->CreateCondBr(element->GetCondition()->Accept(this),
                                                   consequent,
                                                   alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(condition);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  return ifStatement;
}

llvm::Value*
Codegen::Visit(const AssignmentExpression* element) {
  handlingUnsignedVariable = IsUnsigned(element->GetDataType());

  llvm::Type* varType = ResolveType(element->GetDataType());
  llvm::AllocaInst* variableAllocation = builder->CreateAlloca(varType, nullptr, element->GetIdentifier());

  llvm::Value* initialValue = element->GetValue()->Accept(this);
  initialValue->mutateType(varType);

  namedValues[element->GetIdentifier()] = variableAllocation;

  handlingUnsignedVariable = false;

  return builder->CreateStore(initialValue, variableAllocation);
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
    case TK_PLUS:
      return builder->CreateAdd(lhs, rhs, "addtmp");
    case TK_MINUS:
      return builder->CreateSub(lhs, rhs, "subtmp");
    case TK_STAR:
      return builder->CreateMul(lhs, rhs, "multmp");
    case TK_SLASH: {
      if (handlingUnsignedVariable) {
        return builder->CreateUDiv(lhs, rhs, "divtmp");
      } else {
        return builder->CreateSDiv(lhs, rhs, "divtmp");
      }
    }
    case TK_PERCENT: {
      if (handlingUnsignedVariable) {
        return builder->CreateURem(lhs, rhs, "modtmp");
      } else {
        return builder->CreateSRem(lhs, rhs, "modtmp");
      }
    }
    case TK_EQUAL:
      return builder->CreateICmpEQ(lhs, rhs, "eqtmp");
    case TK_NOT_EQUAL:
      return builder->CreateICmpNE(lhs, rhs, "neqtmp");
    default:
      return nullptr;
  }
}

llvm::Value*
Codegen::Visit(const Variable* element) {
  llvm::AllocaInst* alloca = namedValues.at(element->GetIdentifier());

  return builder->CreateLoad(alloca->getAllocatedType(), alloca, element->GetIdentifier());
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