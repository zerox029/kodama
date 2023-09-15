//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_PARSER_PARSER_HPP_
#define KODAMA_SRC_PARSER_PARSER_HPP_

#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include <queue>
#include <utility>
#include <memory>

class Parser {
 public:
  explicit Parser(std::queue<Token> tokens);
  std::shared_ptr<AstNode> Parse();

 private:
  std::queue<Token> tokens;
  Token currentToken;

  void advance();

  std::shared_ptr<AstNode> ParseBinaryOperation();
  std::shared_ptr<AstNode> ParseNumber();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
