//
// Created by emma on 25/09/23.
//

#include "compiler.hpp"
#include "../parser/parser.hpp"
#include "../analyzer/semanticValidator.hpp"
#include "../analyzer/type/typeChecker.hpp"
#include "../codegen/codegen.hpp"
#include "../utils/stringUtils.hpp"

void
Compiler::CheckForErrors(const std::vector<Errors::Error>& errors) {
  if(!errors.empty()) {
    for(const Errors::Error& error : errors) {
      error.Throw();
    }

    exit(1);
  }
}

std::optional<AstNodePtr>
Compiler::Parse(const std::vector<std::string>& code, const std::vector<Token>& tokens) {
  Parser parser{code, tokens};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();
  if(parseResult.index() == 0) {
    return get<AstNodePtr>(parseResult);
  }
  else {
    CheckForErrors(get<std::vector<Errors::Error>>(parseResult));
    return {};
  }
}

void
Compiler::ValidateSemantics(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast) {
  SemanticValidator semanticValidator{code, tokens};
  std::vector<Errors::Error> typeErrors = semanticValidator.Validate(ast);
  CheckForErrors(typeErrors);
}


void
Compiler::TypeCheck(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast) {
  TypeChecker typeChecker{code, tokens};
  std::vector<Errors::Error> typeErrors = typeChecker.TypeCheck(ast);
  CheckForErrors(typeErrors);
}

void
Compiler::GenerateCode(const AstNodePtr& ast) {
  Codegen codegen{};
  codegen.Generate(ast);
  codegen.SaveModuleToFile("../out/out.ll");
}

void
Compiler::Compile(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, "/home/emma/Desktop/Kodama/kodama/code.kdm"};
  std::vector<Token> tokens = lexer.Tokenize();

  AstNodePtr ast = Parse(codeLines, tokens).value();
  //ValidateSemantics(codeLines, tokens, ast);
  //TypeCheck(codeLines, tokens, ast);
  GenerateCode(ast);
}