//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include "../errors/error.hpp"
#include <utility>
#include <iostream>

Parser::Parser(std::vector<std::string> code, std::vector<Token> tokensVec) : code{std::move(code)},
                                                                              tokens{std::move(tokensVec)},
                                                                              currentToken{tokens.at(0)} {}

AstNodePtr
Parser::Parse() {
  std::vector<AstNodePtr> statements;

  while (!IsFinishedParsing()) {
    statements.push_back(ParseFunctionDeclaration() ?: ParseStatement());
  }

  return std::make_shared<Block>(statements);
}

AstNodePtr
Parser::ParseFunctionDeclaration() {
  if (Consume(TK_FN)) {
    std::string identifier = Consume(TK_IDENTIFIER)->GetStr();

    Expect(TK_OPEN_PAREN);
    std::vector<AstNodePtr> parameters = ParseFunctionParameters();
    Expect(TK_CLOSED_PAREN);

    Expect(TK_ARROW);
    DataType dataType = TokenTypeToDataType(ConsumeDataType()->GetTokenType());
    AstNodePtr body = ParseStatement();

    return std::make_shared<FunctionDeclaration>(identifier, parameters, dataType, body);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionParameters() {
  std::vector<AstNodePtr> parameters{};

  do {
    if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON);
      DataType dataType = TokenTypeToDataType(ConsumeDataType()->GetTokenType());

      AstNodePtr parameter = std::make_shared<FunctionParameter>(identifier->GetStr(), dataType);
      parameters.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  return parameters;
}

AstNodePtr
Parser::ParseStatement() {
  if (AstNodePtr ret = ParseReturn()) {
    return ret;
  } else if (AstNodePtr block = ParseBlock()) {
    return block;
  } else if (AstNodePtr ifElseStatement = ParseIfElseStatement()) {
    return ifElseStatement;
  } else if (AstNodePtr whileLoop = ParseWhileLoop()) {
    return whileLoop;
  } else if (AstNodePtr doWhileLoop = ParseDoWhileLoop()) {
    return doWhileLoop;
  } else {
    AstNodePtr expression = ParseExpression();
    Consume(TK_SEMICOLON);

    return expression;
  }
}

AstNodePtr
Parser::ParseReturn() {
  if (Consume(TK_RET)) {
    AstNodePtr returnValue{ParseEqualityExpression()};
    Consume(TK_SEMICOLON);

    return std::make_shared<ReturnStatement>(returnValue);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseBlock() {
  if (Consume(TK_OPEN_CURLY)) {
    std::vector<AstNodePtr> statements;

    while (!Consume(TK_CLOSED_CURLY)) {
      statements.push_back(ParseStatement());
    }

    return std::make_shared<Block>(statements);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseIfElseStatement() {
  if (Consume(TK_IF)) {
    Expect(TK_OPEN_PAREN);
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN);
    AstNodePtr consequent{ParseStatement()};

    if (Consume(TK_ELSE)) {
      AstNodePtr alternative{ParseStatement()};
      return std::make_shared<IfElseStatement>(condition, consequent, alternative);
    }

    return std::make_shared<IfStatement>(condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseWhileLoop() {
  if (Consume(TK_WHILE)) {
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
  if (Consume(TK_DO)) {
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
  if (Consume(TK_DEF)) {
    return ParseAssignment();
  } else {
    return ParseEqualityExpression();
  }
}

AstNodePtr
Parser::ParseAssignment() {
  std::string identifier = Consume(TK_IDENTIFIER)->GetStr();
  Expect(TK_COLON);
  DataType dataType = TokenTypeToDataType(ConsumeDataType()->GetTokenType());
  Expect(TK_ASSIGN);

  return std::make_shared<AssignmentExpression>(identifier, dataType, ParseEqualityExpression());
}

AstNodePtr
Parser::ParseEqualityExpression() {
  AstNodePtr expression = ParseAddExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_EQUAL,
                                                           TK_NOT_EQUAL,
                                                           TK_GREATER,
                                                           TK_LESS,
                                                           TK_GREATER_EQ,
                                                           TK_LESS_EQ})) {
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
  if (AstNodePtr stringNode = ParseString()) {
    return stringNode;
  } else if (AstNodePtr numberNode = ParseNumber()) {
    return numberNode;
  } else if (LookAhead(0, TK_EXTERN) || LookAhead(1, TK_OPEN_PAREN)) {
    return ParseFunctionCall();
  } else if (AstNodePtr identifierNode = ParseIdentifier()) {
    return identifierNode;
  }

  return nullptr;
}

AstNodePtr
Parser::ParseFunctionCall() {
  bool isExtern = static_cast<bool>(Consume(TK_EXTERN));
  std::string identifier = Consume(TK_IDENTIFIER)->GetStr();

  if (Consume(TK_OPEN_PAREN)) {
    std::vector<AstNodePtr> arguments = ParseFunctionArguments();
    Expect(TK_CLOSED_PAREN);

    return std::make_shared<FunctionCall>(identifier, arguments, isExtern);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionArguments() {
  std::vector<AstNodePtr> arguments{};

  do {
    if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON);
      AstNodePtr value = ParseEqualityExpression();

      AstNodePtr parameter = std::make_shared<FunctionArgument>(identifier->GetStr(), value);
      arguments.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  return arguments;
}

AstNodePtr
Parser::ParseNumber() {
  if (std::shared_ptr<Token> integerPortion = Consume(TK_NUMBER)) {
    if (Consume(TK_DOT)) { // Float value
      std::shared_ptr<Token> decimalPortion = Consume(TK_NUMBER);

      return std::make_shared<NumberLiteral>(integerPortion->GetStr(), decimalPortion->GetStr());
    } // Integer value
    else {
      return std::make_shared<NumberLiteral>(integerPortion->GetStr());
    }
  } else {
    return nullptr;
  }
}

AstNodePtr
Parser::ParseString() {
  if (Consume(TK_QUOTATION)) {
    std::string stringValue = Consume(TK_STRING)->GetStr();
    Expect(TK_QUOTATION);

    return std::make_shared<StringLiteral>(stringValue);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseIdentifier() {
  if (std::shared_ptr<Token> identifierNode = Consume(TK_IDENTIFIER)) {
    Variable numberLiteralNode{identifierNode->GetStr()};
    return std::make_shared<Variable>(numberLiteralNode);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Token>
Parser::Consume(TokenType tokenType) {
  if (currentToken.GetTokenType() == tokenType) {
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
  return tokens.at(currentTokenIndex + lookaheadDistance).GetTokenType() == tokenType;
}

std::unique_ptr<Token>
Parser::ConsumeDataType() {
  return ConsumeOneOf({TK_BOOL, TK_U8, TK_U16, TK_U32, TK_U64, TK_U128,
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
  if (currentToken.GetTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    advance();
  } else {

    std::string symbol = Lexer::GetSymbolFromTokenType(tokenType);
    std::string errorMessage = "expected token '" + symbol + "'";
    std::string codeLine = code.at(currentToken.GetLocation().lineNumber);
    Location errorLocation = currentToken.GetLocation();
    errorLocation.characterLineIndex = errorLocation.characterLineIndex - 1;
    Error error{"syntax error", errorMessage, errorLocation, codeLine};
    error.Throw();
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