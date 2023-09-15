//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>

Parser::Parser(std::queue<Token> tokenQueue) : tokens{std::move(tokenQueue)}, currentToken{tokens.front()} {
  tokens.pop();
}

std::shared_ptr<AstNode>
Parser::Parse() {

}

void
Parser::advance() {
  currentToken = tokens.front();
  tokens.pop();
}

std::shared_ptr<AstNode>
Parser::ParseBinaryOperation() {
  std::shared_ptr<AstNode> lhs = ParseNumber();
}

std::shared_ptr<AstNode>
Parser::ParseNumber() {
  NumberLiteral numberLiteralNode{currentToken, currentToken.getStr()};

  advance();

  return std::make_shared<NumberLiteral>(numberLiteralNode);
}