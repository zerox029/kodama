//
// Created by emma on 10/09/23.
//

#include "lexer.hpp"
#include <sstream>

Token Lexer::peek() {
  Token token = next();
  index -= token.getStr().length();

  return token;
}

Token Lexer::next() {
  char c = input.at(index);

  if(isdigit(c))
    return readNumber();

  switch(c) {
    case '+':
      index++;
      return Token{TK_PLUS, "+"};
    case '-':
      index++;
      return Token{TK_MINUS, "-"};
    case '*':
      index++;
      return Token{TK_STAR, "*"};
    case '/':
      index++;
      return Token{TK_SLASH, "/"};
    default:
      index++;
      return Token{TK_EOF, ""};
  }
}

Token Lexer::readNumber() {
  std::stringstream str {};

  while (index < input.length() && isdigit(input.at(index))) {
    str << input.at(index);
    index++;
  }

  return Token{TK_NUMBER, str.str()};
}

std::queue<Token> Lexer::tokenize() {
  std::queue<Token> tokens {};

  while(index < input.length() && peek().getTokenType() != TK_EOF) {
    tokens.push(next());
  }

  return tokens;
}