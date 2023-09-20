//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>
#include <iostream>

Parser::Parser(std::vector<Token> tokensVec) : tokens{std::move(tokensVec)},
                                               currentTokenIndex{0},
                                               currentToken{tokens.at(0)} {}

AstNodePtr
Parser::Parse() {
  std::vector<AstNodePtr> statements;

  while(!IsFinishedParsing()) {
    statements.push_back(ParseFunctionDeclaration() ?: ParseStatement());
  }

  return std::make_shared<Block>(statements);
}

AstNodePtr
Parser::ParseFunctionDeclaration() {
  if(Consume(TK_FN)) {
    std::string identifier = Consume(TK_IDENTIFIER)->getStr();

    Expect(TK_OPEN_PAREN);
    std::vector<AstNodePtr> parameters = ParseFunctionParameters();
    Expect(TK_CLOSED_PAREN);

    Expect(TK_ARROW);
    DataType dataType = TokenTypeToDataType(ConsumeDataType()->getTokenType());
    AstNodePtr body = ParseStatement();

    return std::make_shared<FunctionDeclaration>(identifier, parameters, dataType, body);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionParameters() {
  std::vector<AstNodePtr> parameters {};

  do {
    if(std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON);
      DataType dataType = TokenTypeToDataType(ConsumeDataType()->getTokenType());

      AstNodePtr parameter = std::make_shared<FunctionParameter>(identifier->getStr(), dataType);
      parameters.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  return parameters;
}

AstNodePtr
Parser::ParseStatement() {
  if(AstNodePtr ret = ParseReturn()) {
    return ret;
  }
  else if(AstNodePtr block = ParseBlock()) {
    return block;
  }
  else if(AstNodePtr ifElseStatement = ParseIfElseStatement()) {
    return ifElseStatement;
  }
  else if(AstNodePtr whileLoop = ParseWhileLoop()) {
    return whileLoop;
  }
  else if(AstNodePtr doWhileLoop = ParseDoWhileLoop()) {
    return doWhileLoop;
  }
  else {
    AstNodePtr expression = ParseExpression();
    Consume(TK_SEMICOLON);

    return expression;
  }
}

AstNodePtr
Parser::ParseReturn() {
  if(Consume(TK_RET)) {
    AstNodePtr returnValue{ParseEqualityExpression()};
    Consume(TK_SEMICOLON);

    return std::make_shared<ReturnStatement>(returnValue);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseBlock() {
  if(Consume(TK_OPEN_CURLY)) {
    std::vector<AstNodePtr> statements;

    while(!Consume(TK_CLOSED_CURLY)) {
      statements.push_back(ParseStatement());
    }

    return std::make_shared<Block>(statements);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseIfElseStatement() {
  if(Consume(TK_IF)) {
    Expect(TK_OPEN_PAREN);
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN);
    AstNodePtr consequent{ParseStatement()};

    if(Consume(TK_ELSE)) {
      AstNodePtr alternative{ParseStatement()};
      return std::make_shared<IfElseStatement>(condition, consequent, alternative);
    }

    return std::make_shared<IfStatement>(condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseWhileLoop() {
  if(Consume(TK_WHILE)) {
    Expect(TK_OPEN_PAREN);
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN);
    AstNodePtr consequent{ParseStatement()};

    return std::make_shared<WhileLoop>(condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseDoWhileLoop() {
  if(Consume(TK_DO)) {
    AstNodePtr consequent{ParseStatement()};
    Expect(TK_WHILE);
    Expect(TK_OPEN_PAREN);
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN);

    return std::make_shared<DoWhileLoop>(condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseExpression() {
  if(Consume(TK_LET)) {
    return ParseAssignment();
  }
  else {
    return ParseEqualityExpression();
  }
}

AstNodePtr
Parser::ParseAssignment() {
  std::string identifier = Consume(TK_IDENTIFIER)->getStr();
  Expect(TK_COLON);
  DataType dataType = TokenTypeToDataType(ConsumeDataType()->getTokenType());
  Expect(TK_ASSIGN);

  return std::make_shared<AssignmentExpression>(identifier, dataType, ParseEqualityExpression());
}

AstNodePtr
Parser::ParseEqualityExpression() {
  AstNodePtr expression = ParseAddExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_EQUAL, TK_NOT_EQUAL})) {
    BinaryOperation binaryOperationNode{*operatorToken, expression, ParseAddExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

AstNodePtr
Parser::ParseAddExpression() {
  AstNodePtr expression = ParseMulExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_PLUS, TK_MINUS})) {
    BinaryOperation binaryOperationNode{*operatorToken, expression, ParseMulExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

AstNodePtr
Parser::ParseMulExpression() {
  AstNodePtr expression = ParsePrimaryExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_STAR, TK_SLASH, TK_PERCENT})) {
    BinaryOperation binaryOperationNode{*operatorToken, expression, ParsePrimaryExpression()};
    return std::make_shared<BinaryOperation>(binaryOperationNode);
  }

  return expression;
}

AstNodePtr
Parser::ParsePrimaryExpression() {
  if(AstNodePtr numberNode = ParseNumber()) {
    return numberNode;
  }

  if(LookAhead(1, TK_OPEN_PAREN)) {
    return ParseFunctionCall();
  }
  else if(AstNodePtr identifierNode = ParseIdentifier()) {
    return identifierNode;
  }

  return nullptr;
}

AstNodePtr
Parser::ParseFunctionCall() {
  std::string identifier = Consume(TK_IDENTIFIER)->getStr();

  if(Consume(TK_OPEN_PAREN)) {
    std::vector<AstNodePtr> arguments = ParseFunctionArguments();
    Expect(TK_CLOSED_PAREN);

    return std::make_shared<FunctionCall>(identifier, arguments);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionArguments() {
  std::vector<AstNodePtr> arguments {};

  do {
    if(std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON);
      AstNodePtr value = ParseEqualityExpression();

      AstNodePtr parameter = std::make_shared<FunctionArgument>(identifier->getStr(), value);
      arguments.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  return arguments;
}

AstNodePtr
Parser::ParseNumber() {
  if(std::shared_ptr<Token> integerPortion = Consume(TK_NUMBER)) {
    if(Consume(TK_DOT)) { // Float value
      std::shared_ptr<Token> decimalPortion = Consume(TK_NUMBER);
      NumberLiteral numberLiteralNode{integerPortion->getStr(), decimalPortion->getStr()};

      return std::make_shared<NumberLiteral>(numberLiteralNode);
    } // Integer value
    else {
      NumberLiteral numberLiteralNode{integerPortion->getStr()};

      return std::make_shared<NumberLiteral>(numberLiteralNode);
    }
  }
  else {
    return nullptr;
  }
}

AstNodePtr
Parser::ParseIdentifier() {
  if(std::shared_ptr<Token> identifierNode = Consume(TK_IDENTIFIER)) {
    Variable numberLiteralNode{identifierNode->getStr()};
    return std::make_shared<Variable>(numberLiteralNode);
  }
  else {
    return nullptr;
  }
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

bool
Parser::LookAhead(size_t lookaheadDistance, TokenType tokenType) {
  //TODO: Add bound check
  return tokens.at(currentTokenIndex + lookaheadDistance).getTokenType() == tokenType;
}

std::unique_ptr<Token>
Parser::ConsumeDataType() {
  return ConsumeOneOf({TK_U8, TK_U16, TK_U32, TK_U64, TK_U128,
                                            TK_I8, TK_I16, TK_I32, TK_I64, TK_I128, TK_F32, TK_F64});
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

bool
Parser::IsFinishedParsing() {
  return currentTokenIndex >= tokens.size() - 1;
}