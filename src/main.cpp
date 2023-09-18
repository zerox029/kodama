#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include <llvm/IR/Value.h>
#include <iostream>

void
run() {
  Lexer lexer{"fn main() -> i64 { return 0; }"};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{tokens};
  AstNodePtr tree = parser.Parse();

  Codegen codegen{};
  codegen.Generate(tree);
  codegen.Print();
  codegen.saveModuleToFile("../out/out.ll");
}

int
main() {
  run();
  return 0;
}
