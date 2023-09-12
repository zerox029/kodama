#include <iostream>
#include "lexer/lexer.hpp"

int main() {
  Lexer lexer {"14+5"};

  std::queue<Token> tokens = lexer.tokenize();

  std::cout << tokens.size() << std::endl;

  return 0;
}
