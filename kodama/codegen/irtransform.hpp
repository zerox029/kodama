//
// Created by emma on 17/09/23.
//

#ifndef KODAMA_SRC_CODEGEN_IRTRANSFORM_HPP_
#define KODAMA_SRC_CODEGEN_IRTRANSFORM_HPP_

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

class IRTransform {
 public:
  static void FixEmptyBlocks(std::unique_ptr<llvm::Module>& module, std::unique_ptr<llvm::IRBuilder<>>& builder);
};

#endif //KODAMA_SRC_CODEGEN_IRTRANSFORM_HPP_
