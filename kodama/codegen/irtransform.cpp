//
// Created by emma on 17/09/23.
//

#include "irtransform.hpp"

void //Return void from empty blocks(temporary behavior)
IRTransform::FixEmptyBlocks(std::unique_ptr<llvm::Module>& module, std::unique_ptr<llvm::IRBuilder<>>& builder) {
  for (auto& function : *module) {
    for (auto basicBlockIterator = function.begin(); basicBlockIterator != function.end();) {
      llvm::BasicBlock* basicBlock = &(*basicBlockIterator);

      if (basicBlock->empty()) {
        builder->SetInsertPoint(basicBlock);
        builder->CreateRet(nullptr);
      } else {
        ++basicBlockIterator;
      }
    }
  }
}