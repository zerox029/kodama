#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "codegen/codegen.hpp"

int main() {
  Lexer lexer{"14 + 5"};
  std::queue<Token> tokens = lexer.tokenize();

  Parser parser{tokens};
  std::shared_ptr<AstNode> tree = parser.Parse();

  Codegen codegen{};
  tree->Accept(&codegen);

  return 0;
}
