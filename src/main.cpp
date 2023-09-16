#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include <llvm/IR/Value.h>

void run() {
  Lexer lexer{"10%4"};
  std::queue<Token> tokens = lexer.tokenize();

  Parser parser{tokens};
  std::shared_ptr<AstNode> tree = parser.Parse();

  Codegen codegen{};
  codegen.Generate(tree);
  codegen.Print();
}

int main() {
  run();
  return 0;
}
