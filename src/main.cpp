#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include <llvm/IR/Value.h>
#include <iostream>

void
run() {
  Lexer lexer{"return 1!=1"};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{tokens};
  std::shared_ptr<AstNode> tree = parser.Parse();

  Codegen codegen{};
  codegen.Generate(tree);
  codegen.Print();
}

int
main() {
  run();
  return 0;
}
