//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_LEXER_HPP_
#define KODAMA_SRC_LEXER_LEXER_HPP_

#include "token.hpp"
#include <queue>
#include <string>

class Lexer {
 public:
  explicit Lexer(const std::string& input) : input{input}, index{0} {}

  std::queue<Token> tokenize();

 private:
  std::string_view input;
  size_t index;

  Token peek();
  Token next();
  Token readNumber();
};

#endif //KODAMA_SRC_LEXER_LEXER_HPP_
