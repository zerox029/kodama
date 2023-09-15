#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#include <iostream>

int main() {
  Lexer lexer{"14 + 5"};
  std::queue<Token> tokens = lexer.tokenize();

  Parser parser{tokens};
  std::shared_ptr<AstNode> tree = parser.Parse();

  std::cout << tree->Stringify();

  return 0;
}
