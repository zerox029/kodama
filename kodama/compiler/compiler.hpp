//
// Created by emma on 25/09/23.
//

#ifndef KODAMA_KODAMA_COMPILER_COMPILER_HPP_
#define KODAMA_KODAMA_COMPILER_COMPILER_HPP_

#include <vector>
#include <string>
#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include "../errors/error.hpp"

class Compiler {
  void CheckForErrors(const std::vector<Error>& errors);
  std::optional<AstNodePtr> Parse(const std::vector<std::string>& code, const std::vector<Token>& tokens);
  void ValidateSemantics(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast);
  void TypeCheck(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast);
  void GenerateCode(const AstNodePtr& ast);

 public:
  void Compile(const std::string& code);
};


#endif //KODAMA_KODAMA_COMPILER_COMPILER_HPP_
