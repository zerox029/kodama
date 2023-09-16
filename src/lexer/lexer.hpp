//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_LEXER_HPP_
#define KODAMA_SRC_LEXER_LEXER_HPP_

#include "token.hpp"
#include <queue>
#include <string>
#include <optional>
#include <utility>

class Lexer {
 public:
  explicit Lexer(std::string input) : input{std::move(input)}, index{0} {}

  std::queue<Token> tokenize();

 private:
  std::string input;
  size_t index;

  Token peek();
  Token next();
  std::optional<Token> readSymbol();
  std::optional<Token> readKeyword();
  std::optional<Token> readNumber();
};

#endif //KODAMA_SRC_LEXER_LEXER_HPP_
