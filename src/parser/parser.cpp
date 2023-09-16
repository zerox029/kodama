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
  return ParseAdditiveExpression();
}

std::shared_ptr<AstNode>
Parser::ParseAdditiveExpression() {
  std::shared_ptr<AstNode> expression = ParseMultiplicativeExpression();

  if(currentToken.getTokenType() == TK_PLUS || currentToken.getTokenType() == TK_MINUS) {
    Token operatorToken = currentToken;
    advance();
    BinaryOperation binaryOperationNode {operatorToken, expression, ParseMultiplicativeExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

std::shared_ptr<AstNode>
Parser::ParseMultiplicativeExpression() {
  std::shared_ptr<AstNode> expression = ParseNumber();

  if(currentToken.getTokenType() == TK_STAR || currentToken.getTokenType() == TK_SLASH) {
    Token operatorToken = currentToken;
    advance();
    BinaryOperation binaryOperationNode {operatorToken, expression, ParseNumber()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

std::shared_ptr<AstNode>
Parser::ParseNumber() {
  NumberLiteral numberLiteralNode{currentToken, currentToken.getStr()};
  advance();

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