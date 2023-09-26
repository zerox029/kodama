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
  currentVariableType = nullptr;
  currentFunctionType = nullptr;
}

void
Codegen::Print() {
  module->print(llvm::outs(), nullptr);
}

void
Codegen::SaveModuleToFile(const std::string& fileName) {
  std::error_code errorCode;
  llvm::raw_fd_ostream outLL(fileName, errorCode);
  module->print(outLL, nullptr);
}

void
Codegen::Generate(const AstNodePtr& ast) {
  setupExternFunctions();
  ast->Accept(this);
}

void
Codegen::Visit(Program* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
Codegen::Visit(FunctionDeclaration* element) {
  currentFunctionType = element->GetReturnType();

  std::vector<AstNodePtr> parameterNodes = element->GetParameters();
  std::vector<llvm::Type*> parameters{element->GetParameters().size()};
  std::transform(parameterNodes.begin(),
                 parameterNodes.end(),
                 parameters.begin(),
                 [this](const AstNodePtr& node) {
                   return (std::static_pointer_cast<FunctionParameter>(node))->GetDataType()->GetLLVMType(*context);
                 });

  llvm::FunctionType* functionType = llvm::FunctionType::get(element->GetReturnType()->GetLLVMType(*context),
                                                             parameters,
                                                             false);

  llvm::Function* function = CreateFunction(element->GetIdentifier(),
                                            functionType,
                                            element->GetParameters());

  namedValues.clear();
  for (auto& argument : function->args()) {
    llvm::AllocaInst* alloca = builder->CreateAlloca(argument.getType(),
                                                     nullptr,
                                                     argument.getName());
    builder->CreateStore(&argument, alloca);

    // Add arguments to variable symbol table.
    namedValues[std::string(argument.getName())] = alloca;
  }

  element->GetBody()->Accept(this);

  lastGeneratedValue = function;
  currentFunctionType = nullptr;
}

void
Codegen::Visit(FunctionParameter* element) {

}

void
Codegen::Visit(Block* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
    node->Accept(this);
  }
}

void
Codegen::Visit(ReturnStatement* element) {
  element->GetReturnValue()->Accept(this);
  llvm::Value* returnValue = lastGeneratedValue;

  lastGeneratedValue = builder->CreateRet(returnValue);
}

void
Codegen::Visit(IfStatement* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  element->GetCondition()->Accept(this);
  llvm::Value* conditionValue = lastGeneratedValue;
  llvm::Value* ifStatement = builder->CreateCondBr(conditionValue,
                                                   consequent,
                                                   alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(alternative);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  lastGeneratedValue = ifStatement;
}

void
Codegen::Visit(IfElseStatement* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);
  llvm::BasicBlock* merge = llvm::BasicBlock::Create(*context, "merge", fn);

  element->GetCondition()->Accept(this);
  llvm::Value* conditionValue = lastGeneratedValue;
  llvm::Value* ifStatement = builder->CreateCondBr(conditionValue,
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

  lastGeneratedValue = ifStatement;
}

void
Codegen::Visit(WhileLoop* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* condition = llvm::BasicBlock::Create(*context, "condition", fn);
  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  // Generate the condition
  builder->CreateBr(condition);
  builder->SetInsertPoint(condition);

  element->GetCondition()->Accept(this);
  llvm::Value* conditionValue = lastGeneratedValue;

  llvm::Value* whileLoop = builder->CreateCondBr(conditionValue,
                                                 consequent,
                                                 alternative);

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(condition);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  lastGeneratedValue = whileLoop;
}

void
Codegen::Visit(DoWhileLoop* element) {
  llvm::Function* fn = module->getFunction("main");

  llvm::BasicBlock* condition = llvm::BasicBlock::Create(*context, "condition", fn);
  llvm::BasicBlock* consequent = llvm::BasicBlock::Create(*context, "consequent", fn);
  llvm::BasicBlock* alternative = llvm::BasicBlock::Create(*context, "alternative", fn);

  // Generate the condition
  builder->CreateBr(consequent);
  builder->SetInsertPoint(condition);

  element->GetCondition()->Accept(this);
  llvm::Value* conditionValue = lastGeneratedValue;

  builder->CreateCondBr(conditionValue,
                        consequent,
                        alternative);
  llvm::Value* whileLoop = lastGeneratedValue;

  // Generate the consequent
  builder->SetInsertPoint(consequent);
  element->GetConsequent()->Accept(this);
  builder->CreateBr(condition);

  // Go to the merge point
  builder->SetInsertPoint(alternative);

  lastGeneratedValue = whileLoop;
}

void
Codegen::Visit(AssignmentExpression* element) {
  currentVariableType = element->GetDataType();
  llvm::Type* varType = element->GetDataType()->GetLLVMType(*context);

  if (element->GetDataType()->GetTypeName() == STRING_TYPE) {
    element->GetValue()->Accept(this);

    std::string dataLayoutStr = module->getDataLayoutStr();
    llvm::DataLayout dataLayout(dataLayoutStr);

    llvm::TypeSize size = dataLayout.getTypeAllocSize(lastGeneratedValue->getType());
    llvm::AllocaInst
        * variableAllocation = builder->CreateAlloca(varType, builder->getInt8(size), element->GetIdentifier());

    namedValues[element->GetIdentifier()] = variableAllocation;

    lastGeneratedValue = builder->CreateStore(lastGeneratedValue, variableAllocation);
  } else {
    element->GetValue()->Accept(this);

    llvm::AllocaInst* variableAllocation = builder->CreateAlloca(varType, nullptr, element->GetIdentifier());
    lastGeneratedValue = builder->CreateStore(lastGeneratedValue, variableAllocation);
    namedValues[element->GetIdentifier()] = variableAllocation;
  }

  currentVariableType = nullptr;
}

void
Codegen::Visit(ReassignmentExpression* element) {

}

void
Codegen::Visit(BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  llvm::Value* lhs = lastGeneratedValue;
  element->GetRhs()->Accept(this);
  llvm::Value* rhs = lastGeneratedValue;

  switch (element->GetOperator().GetTokenType()) {
    case TK_PLUS:
      lastGeneratedValue = builder->CreateAdd(lhs, rhs, "addtmp");
      break;
    case TK_MINUS:
      lastGeneratedValue = builder->CreateSub(lhs, rhs, "subtmp");
      break;
    case TK_STAR:
      lastGeneratedValue = builder->CreateMul(lhs, rhs, "multmp");
      break;
    case TK_SLASH: {
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateUDiv(lhs, rhs, "divtmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateSDiv(lhs, rhs, "divtmp");
        break;
      }
    }
    case TK_PERCENT: {
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateURem(lhs, rhs, "modtmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateSRem(lhs, rhs, "modtmp");
        break;
      }
    }
    case TK_EQUAL:
      lastGeneratedValue = builder->CreateICmpEQ(lhs, rhs, "eqtmp");
      break;
    case TK_NOT_EQUAL:
      lastGeneratedValue = builder->CreateICmpNE(lhs, rhs, "neqtmp");
      break;
    case TK_GREATER:
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateICmpUGT(lhs, rhs, "gttmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateICmpSGT(lhs, rhs, "gttmp");
        break;
      }
    case TK_LESS:
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateICmpULT(lhs, rhs, "lttmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateICmpSLT(lhs, rhs, "lttmp");
        break;
      }
    case TK_GREATER_EQ:
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateICmpUGE(lhs, rhs, "gtetmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateICmpSGE(lhs, rhs, "gtetmp");
        break;
      }
    case TK_LESS_EQ:
      if (currentVariableType->GetTypeCategory() == UINTEGER) {
        lastGeneratedValue = builder->CreateICmpULE(lhs, rhs, "ltetmp");
        break;
      } else {
        lastGeneratedValue = builder->CreateICmpSLE(lhs, rhs, "ltetmp");
        break;
      }
    default:
      break;
  }
}

void
Codegen::Visit(FunctionCall* element) {
  // Look up the name in the global module table.
  llvm::Function* callee = module->getFunction(element->GetIdentifier());
  if (!callee) {
    fprintf(stderr, "Error: %s\n", "Unknown function referenced");
    exit(1);
  }

  // If argument mismatch error.
  if (callee->arg_size() != element->GetArguments().size()) {
    fprintf(stderr, ("Incorrect # arguments passed"));
    exit(1);
  }

  std::vector<llvm::Value*> args;
  for (auto& argument : element->GetArguments()) {
    argument->Accept(this);
    args.push_back(lastGeneratedValue);
  }

  lastGeneratedValue = builder->CreateCall(callee, args, "calltmp");
}

void
Codegen::Visit(FunctionArgument* element) {
  element->GetValue()->Accept(this);
}

void
Codegen::Visit(Variable* element) {
  llvm::AllocaInst* alloca = namedValues.at(element->GetIdentifier());

  lastGeneratedValue = builder->CreateLoad(alloca->getAllocatedType(), alloca, element->GetIdentifier());
}

void
Codegen::Visit(IntegerLiteral* element) {
  if (currentVariableType == nullptr) {
    lastGeneratedValue = llvm::ConstantInt::get(*context, llvm::APInt(64, element->GetValue(), 10));
  } else {
    lastGeneratedValue = llvm::ConstantInt::get(*context, llvm::APInt(32, element->GetValue(), 10));
  }
}

void
Codegen::Visit(DecimalLiteral* element) {
  lastGeneratedValue = llvm::ConstantFP::get(*context, llvm::APFloat(std::stof(element->GetValue())));
}

void
Codegen::Visit(StringLiteral* element) {
  lastGeneratedValue = builder->CreateGlobalStringPtr(element->GetValue());
  //lastGeneratedValue = llvm::ConstantDataArray::getString(*context, "Hello, LLVM!");
}

void
Codegen::Visit(BoolValue* element) {
  lastGeneratedValue = llvm::ConstantInt::get(*context, llvm::APInt(1, element->GetValue()));
}

void
Codegen::Visit(NullValue* element) {
  llvm::Value* nullPtr = llvm::ConstantPointerNull::get(currentVariableType->GetLLVMType(*context)->getPointerTo());
  lastGeneratedValue = nullPtr;
}

llvm::Function*
Codegen::CreateFunction(const std::string& fnName, llvm::FunctionType* fnType, std::vector<AstNodePtr> parameters) {
  llvm::Function* function = module->getFunction(fnName);

  if (function == nullptr) {
    function = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, fnName, *module);

    unsigned idx = 0;
    for (auto& parameter : function->args()) {
      std::string identifier = (std::static_pointer_cast<FunctionParameter>((parameters.at(idx++))))->GetIdentifier();
      parameter.setName(identifier);
    }

    llvm::verifyFunction(*function);
  }

  llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, "entry", function);
  builder->SetInsertPoint(entry);

  return function;
}

void
Codegen::setupExternFunctions() {
  llvm::PointerType* bytePtrTy = builder->getInt8Ty()->getPointerTo();

  module->getOrInsertFunction("printf", llvm::FunctionType::get(builder->getInt32Ty(), bytePtrTy, true));
}