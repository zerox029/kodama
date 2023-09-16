//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>
#include <iostream>

Parser::Parser(std::vector<Token> tokensVec) : tokens{std::move(tokensVec)},
                                               currentTokenIndex{0},
                                               currentToken{tokens.at(0)} {}

std::shared_ptr<AstNode>
Parser::Parse() {
  return ParseAssignment();
}

std::shared_ptr<AstNode>
Parser::ParseAssignment() {
  Expect(TK_LET);
  std::string identifier = Consume(TK_IDENTIFIER)->getStr();
  Expect(TK_COLON);
  DataType dataType = TokenTypeToDataType(ConsumeDataType()->getTokenType());
  Expect(TK_ASSIGN);

  return std::make_shared<Assignment>(identifier, dataType, ParseAddExpression());
}

std::shared_ptr<AstNode>
Parser::ParseAddExpression() {
  std::shared_ptr<AstNode> expression = ParseMulExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_PLUS, TK_MINUS})) {
    BinaryOperation binaryOperationNode{*operatorToken, expression, ParseMulExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

std::shared_ptr<AstNode>
Parser::ParseMulExpression() {
  std::shared_ptr<AstNode> expression = ParseNumber();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_STAR, TK_SLASH, TK_PERCENT})) {
    BinaryOperation binaryOperationNode{*operatorToken, expression, ParseNumber()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

std::shared_ptr<AstNode>
Parser::ParseNumber() {
  std::shared_ptr<Token> numberNode = Consume(TK_NUMBER);
  NumberLiteral numberLiteralNode{numberNode->getStr()};

  return std::make_shared<NumberLiteral>(numberLiteralNode);
}

std::unique_ptr<Token>
Parser::Consume(TokenType tokenType) {
  if (currentToken.getTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    advance();

    return consumedToken;
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token>
Parser::ConsumeDataType() {
  return ConsumeOneOf({TK_U8, TK_U16, TK_U32, TK_U64, TK_U128});
}

std::unique_ptr<Token>
Parser::ConsumeOneOf(const std::list<TokenType>& possibleTokenTypes) {
  for (const TokenType& tokenType : possibleTokenTypes) {
    std::unique_ptr<Token> consumedToken = Consume(tokenType);
    if (consumedToken) {
      return consumedToken;
    }
  }

  return nullptr;
}

void
Parser::Expect(TokenType tokenType) {
  if (currentToken.getTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    advance();
  } else {
    throw std::invalid_argument("Token was not of expected dataType");
  }
}

void
Parser::advance() {
  if (currentTokenIndex < tokens.size() - 1) {
    currentTokenIndex++;
    currentToken = tokens.at(currentTokenIndex);
  } else {
    std::cout << "No more tokens" << std::endl;
  }
}

DataType
Parser::TokenTypeToDataType(TokenType tokenType) {
  switch (tokenType) {
    case TK_U8:
      return U8;
    case TK_U16:
      return U16;
    case TK_U32:
      return U32;
    case TK_U64:
      return U64;
    case TK_U128:
      return U128;
    default:
      throw std::invalid_argument("Provided token dataType was not a valid data dataType");
  }
}