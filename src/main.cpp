#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include "utils/stringUtils.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
Compile(const std::string& code) {
  Lexer lexer{code, "/home/emma/Desktop/Kodama/src/code.kdm"};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{SplitString(code, "\n"), tokens};
  AstNodePtr tree = parser.Parse();

  Codegen codegen{};
  codegen.Generate(tree);
  codegen.saveModuleToFile("../out/out.ll");

  system("lli ../out/out.ll");
}

int
main() {
  Compile(ReadFile());
  //Error error{"syntax error", "expected '('", {"/a/file/location/code.kdm", 4,20}, "printf(\"hello world\""};
  //error.Throw();

  return 0;
}
