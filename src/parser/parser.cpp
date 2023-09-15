//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>
#include <iostream>

Parser::Parser(std::queue<Token> tokenQueue) : tokens{std::move(tokenQueue)}, currentToken{tokens.front()} {
  tokens.pop();
}

std::shared_ptr<AstNode>
Parser::Parse() {
  return ParseBinaryOperation();
}

std::shared_ptr<AstNode>
Parser::ParseBinaryOperation() {
  std::shared_ptr<AstNode> lhs = ParseNumber();
  advance();
  Token operatorToken = currentToken;
  advance();
  std::shared_ptr<AstNode> rhs = ParseNumber();

  BinaryOperation binaryOperationNode {operatorToken, lhs, rhs};
  return std::make_shared<BinaryOperation>(binaryOperationNode);
}

std::shared_ptr<AstNode>
Parser::ParseNumber() {
  NumberLiteral numberLiteralNode{currentToken, currentToken.getStr()};

  return std::make_shared<NumberLiteral>(numberLiteralNode);
}

void
Parser::advance() {
  if(!tokens.empty()) {
    currentToken = tokens.front();
    tokens.pop();
  }
  else {
    std::cout << "Token stack empty" << std::endl;
  }
}