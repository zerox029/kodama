//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_PARSER_PARSER_HPP_
#define KODAMA_SRC_PARSER_PARSER_HPP_

#include "../lexer/token.hpp"
#include <queue>
#include <utility>

class Parser {
 public:
  explicit Parser(std::queue<Token> tokens) : tokens{std::move(tokens)} {}

  

 private:
  std::queue<Token> tokens;
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
