#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include <llvm/IR/Value.h>
#include <iostream>

void
run() {
  Lexer lexer{"fn add() -> i32 { return 2+2; } fn main() -> i32 { return add(); }     "};
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
