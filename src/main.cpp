#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "analyzer/type/typeChecker.hpp"
#include "codegen/codegen.hpp"
#include "utils/stringUtils.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <variant>

std::string
ReadFile() {
  std::ifstream inputFile("../src/code.kdm");
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file.\n";
    return "";
  }

  std::string line;
  std::stringstream codeStream;
  while (std::getline(inputFile, line)) {
    codeStream << line << "\n";
  }

  inputFile.close();

  return codeStream.str();
}

void
CheckForErrors(const std::vector<Error>& errors) {
  if(!errors.empty()) {
    for(const Error& error : errors) {
      error.Throw();
    }

    exit(1);
  }
}

std::optional<AstNodePtr>
Parse(const std::vector<std::string>& code, const std::vector<Token>& tokens) {
  Parser parser{code, tokens};
  std::variant<AstNodePtr, std::vector<Error>> parseResult = parser.Parse();
  if(parseResult.index() == 0) {
    return get<AstNodePtr>(parseResult);
  }
  else {
    CheckForErrors(get<std::vector<Error>>(parseResult));
    return {};
  }
}

void
TypeCheck(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast) {
  TypeChecker typeChecker{code, tokens};
  std::vector<Error> typeErrors = typeChecker.TypeCheck(ast);
  CheckForErrors(typeErrors);
}

void
GenerateCode(const AstNodePtr& ast) {
  Codegen codegen{};
  codegen.Generate(ast);
  codegen.SaveModuleToFile("../out/out.ll");

  system("lli ../out/out.ll");
}

void
Compile(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, "/home/emma/Desktop/Kodama/src/code.kdm"};
  std::vector<Token> tokens = lexer.Tokenize();

  AstNodePtr ast = Parse(codeLines, tokens).value();
  TypeCheck(codeLines, tokens, ast);
  GenerateCode(ast);
}

int
main() {
  Compile(ReadFile());

  return 0;
}
