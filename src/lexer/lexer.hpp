//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_LEXER_HPP_
#define KODAMA_SRC_LEXER_LEXER_HPP_

#include "token.hpp"
#include <vector>
#include <string>
#include <optional>
#include <utility>

class Lexer {
 public:
  explicit Lexer(std::string input) : input{std::move(input)}, index{0} {}

  std::vector<Token> Tokenize();

 private:
  std::string input;
  size_t index;

  Token Peek();
  Token Next();
  std::optional<Token> ReadSymbol();
  std::optional<Token> ReadKeyword();
  std::optional<Token> ReadNumber();
  std::optional<Token> ReadIdentifier();
};

#endif //KODAMA_SRC_LEXER_LEXER_HPP_
