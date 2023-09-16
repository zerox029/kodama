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
  explicit Parser(std::vector<Token> tokens);
  std::shared_ptr<AstNode> Parse();

 private:
  std::vector<Token> tokens;
  size_t currentTokenIndex;
  Token currentToken;

  std::shared_ptr<AstNode> ParseAddExpression();
  std::shared_ptr<AstNode> ParseMulExpression();

  std::shared_ptr<AstNode> ParseNumber();

  void advance();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
