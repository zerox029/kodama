//
// Created by emma on 14/09/23.
//

#include "codegen.hpp"
#include "irtransform.hpp"
#include <iostream>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

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
Codegen::saveModuleToFile(const std::string& fileName) {
  std::error_code errorCode;
  llvm::raw_fd_ostream outLL(fileName, errorCode);
  module->print(outLL, nullptr);
}

void
Codegen::Generate(const AstNodePtr& ast) {
  setupExternFunctions();
  ast->Accept(this);
}

llvm::Value*
Codegen::Visit(const FunctionDeclaration* element) {
  std::vector<AstNodePtr> parameterNodes = element->GetParameters();
  std::vector<llvm::Type*> parameters{element->GetParameters().size()};
  std::transform(parameterNodes.begin(),
                 parameterNodes.end(),
                 parameters.begin(),
                 [this](const AstNodePtr& node) {
    DataType type = (std::static_pointer_cast<FunctionParameter>(node))->GetDataType();
    return ResolveLLVMType(type);
  });

  llvm::FunctionType* functionType = llvm::FunctionType::get(ResolveLLVMType(element->GetReturnType()),
                                                             parameters,
                                                             false);

  llvm::Function* function = CreateFunction(element->GetIdentifier(),
                                            functionType,
                                            element->GetParameters());

  namedValues.clear();
  for (auto &argument : function->args()) {
    llvm::AllocaInst* alloca = builder->CreateAlloca(argument.getType(),
                                                     nullptr,
                                                     argument.getName());
    builder->CreateStore(&argument, alloca);

    // Add arguments to variable symbol table.
    namedValues[std::string(argument.getName())] = alloca;
  }

  element->GetBody()->Accept(this);

  return function;
}

llvm::Value*
Codegen::Visit(const FunctionParameter* element) {
  return nullptr;
}

llvm::Value*
Codegen::Visit(const Block* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
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

  llvm::Type* varType = ResolveLLVMType(element->GetDataType());
  llvm::AllocaInst* variableAllocation = builder->CreateAlloca(varType, nullptr, element->GetIdentifier());

  llvm::Value* initialValue = element->GetValue()->Accept(this);
  initialValue->mutateType(varType);

  namedValues[element->GetIdentifier()] = variableAllocation;

  handlingUnsignedVariable = false;

  return builder->CreateStore(initialValue, variableAllocation);
}

llvm::Value*
Codegen::Visit(const BinaryOperation* element) {
  llvm::Value* lhs = element->GetLhs()->Accept(this);
  llvm::Value* rhs = element->GetRhs()->Accept(this);

  lhs->print(llvm::outs());
  std::cout << "\n";
  rhs->print(llvm::outs());
  std::cout << "\n";

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
Codegen::Visit(const FunctionCall* element) {
  // Look up the name in the global module table.
  llvm::Function* callee = module->getFunction(element->GetIdentifier());
  if (!callee) {
    fprintf(stderr, "Error: %s\n", "Unknown function referenced");
    return nullptr;
  }

  // If argument mismatch error.
  if (callee->arg_size() != element->GetArguments().size()) {
    fprintf(stderr,("Incorrect # arguments passed"));
    return nullptr;
  }

  std::vector<llvm::Value *> args;
  for (auto & argument : element->GetArguments()) {
    args.push_back(argument->Accept(this));
  }

  return builder->CreateCall(callee, args, "calltmp");
}

llvm::Value*
Codegen::Visit(const FunctionArgument* element) {
  return element->GetValue()->Accept(this);
}

llvm::Value*
Codegen::Visit(const Variable* element) {
  llvm::AllocaInst* alloca = namedValues.at(element->GetIdentifier());

  return builder->CreateLoad(alloca->getAllocatedType(), alloca, element->GetIdentifier());
}

llvm::Value*
Codegen::Visit(const NumberLiteral* element) {
  if(element->GetDecimalValue().empty()) {
    return llvm::ConstantInt::get(*context, llvm::APInt(64, element->GetValue(), 10));
  }
  else {
    return llvm::ConstantFP::get(*context, llvm::APFloat(std::stof(element->GetValue())));
  }
}

llvm::Value*
Codegen::Visit(const StringLiteral* element) {
  return builder->CreateGlobalStringPtr(element->GetValue() + "\n");
}

llvm::Type*
Codegen::ResolveLLVMType(const DataType type) {
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
    case F32:
      return llvm::Type::getFloatTy(*context);
    case F64:
      return llvm::Type::getDoubleTy(*context);
    default:
      return nullptr;
  }
}

llvm::Function*
Codegen::CreateFunction(const std::string& fnName, llvm::FunctionType* fnType, std::vector<AstNodePtr> parameters) {
  llvm::Function* function = module->getFunction(fnName);

  if (function == nullptr) {
    function = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);

    unsigned idx = 0;
    for (auto& parameter : function->args())
    {
      std::string identifier = (std::static_pointer_cast<FunctionParameter>((parameters.at(idx++))))->GetIdentifier();
      parameter.setName(identifier);
    }

    llvm::verifyFunction(*function);
  }

  llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, "entry", function);
  builder->SetInsertPoint(entry);

  return function;
}

void Codegen::setupExternFunctions() {
  llvm::PointerType* bytePtrTy = builder->getInt8Ty()->getPointerTo();

  module->getOrInsertFunction("printf", llvm::FunctionType::get(builder->getInt32Ty(), bytePtrTy, true));
}