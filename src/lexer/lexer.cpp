//
// Created by emma on 10/09/23.
//

#include "lexer.hpp"
#include <sstream>
#include <map>
#include <iostream>

const std::map<std::string, TokenType> symbols = {
    {"+", TK_PLUS}, {"-", TK_MINUS}, {"*", TK_STAR}, {"/", TK_SLASH}, {":", TK_COLON},
    {"=", TK_ASSIGN}
};

const std::map<std::string, TokenType> keywords = {
    {"let", TK_LET}
};

Token Lexer::peek() {
  Token token = next();
  index -= token.getStr().length();

  return token;
}

Token Lexer::next() {
  // Eliminate white spaces
  while(isspace(static_cast<unsigned char>(input.at(index)))) {
    index++;
  }

  // Process symbols
  std::optional<Token> token = readSymbol();
  if(token.has_value()) {
    index += token.value().getStr().length();
    return token.value();
  }


  // Process numbers
  token = readNumber();
  if(token.has_value()) return token.value();

  throw std::invalid_argument{"Unexpected operatorToken"};
}

std::optional<Token> Lexer::readSymbol() {
  auto iterator = symbols.begin();
  while(iterator != symbols.end()) {
    if(input.substr(index).starts_with(iterator->first)) {
      return Token{ iterator->second, iterator->first };
    }
    ++iterator;
  }

  return {};
}

std::optional<Token> Lexer::readKeyword() {
  auto iterator = keywords.begin();
  while(iterator != symbols.end()) {
    if(input.substr(index).starts_with(iterator->first)) {
      return Token{ iterator->second, iterator->first };
    }
    ++iterator;
  }

  return {};
}

std::optional<Token> Lexer::readNumber() {
  std::stringstream numValue {};

  while (index < input.length() && isdigit(static_cast<unsigned char>(input.at(index)))) {
    numValue << input.at(index);
    index++;
  }

  if(numValue.str().length() == 0)
    return {};

  return Token{TK_NUMBER, numValue.str()};
}

std::queue<Token> Lexer::tokenize() {
  std::queue<Token> tokens {};

  while(index < input.length() && peek().getTokenType() != TK_EOF) {
    tokens.push(next());
  }

  return tokens;
}