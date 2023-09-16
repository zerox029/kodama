//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>
#include <iostream>

Parser::Parser(std::vector<Token> tokensVec) : tokens{std::move(tokensVec)}, currentTokenIndex{0}, currentToken{tokens.at(0)} {}

std::shared_ptr<AstNode>
Parser::Parse() {
  return ParseAddExpression();
}

std::shared_ptr<AstNode>
Parser::ParseAddExpression() {
  std::shared_ptr<AstNode> expression = ParseMulExpression();

  if(currentToken.getTokenType() == TK_PLUS || currentToken.getTokenType() == TK_MINUS) {
    Token operatorToken = currentToken;
    advance();
    BinaryOperation binaryOperationNode {operatorToken, expression, ParseMulExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

std::shared_ptr<AstNode>
Parser::ParseMulExpression() {
  std::shared_ptr<AstNode> expression = ParseNumber();

  if(currentToken.getTokenType() == TK_STAR || currentToken.getTokenType() == TK_SLASH || currentToken.getTokenType() == TK_PERCENT) {
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
  if(currentTokenIndex < tokens.size() - 1) {
    currentTokenIndex++;
    currentToken = tokens.at(currentTokenIndex);
  }
  else {
    std::cout << "No more tokens" << std::endl;
  }
}