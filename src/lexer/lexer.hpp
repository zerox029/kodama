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
  explicit Lexer(std::string input) : input{std::move(input)} {}

  std::vector<Token> Tokenize();

 private:
  std::string input;
  size_t index = 0;

  size_t lineNumber = 0;
  size_t characterLineIndex = 0;

  bool isLexingString = false;
  bool lastTokenIsString = false;

  Token Peek();
  Token Next();
  bool isNewline();
  std::optional<Token> ReadSymbol();
  std::optional<Token> ReadKeyword();
  std::optional<Token> ReadNumber();
  std::optional<Token> ReadIdentifier();
  Token ReadString();
};

#endif //KODAMA_SRC_LEXER_LEXER_HPP_
