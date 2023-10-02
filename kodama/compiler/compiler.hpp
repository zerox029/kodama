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
#include "../cli/cli.hpp"

class Compiler {
  void CheckForErrors(const std::vector<Errors::Error>& errors);
  std::optional<std::vector<Token>> Lex(const std::string& code, const std::vector<std::string> codeLines);
  std::optional<AstNodePtr> Parse(const std::vector<std::string>& code, const std::vector<Token>& tokens);
  void ValidateSemantics(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast);
  void TypeCheck(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast);
  void GenerateCode(const AstNodePtr& ast, const std::string& location);

 public:
  void Compile(const cli::CliState& state);
};


#endif //KODAMA_KODAMA_COMPILER_COMPILER_HPP_
