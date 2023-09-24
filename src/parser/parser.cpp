//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include "../errors/error.hpp"
#include "../errors/errorFactory.hpp"
#include <utility>
#include <iostream>
#include <variant>
#include <functional>

Parser::Parser(std::vector<std::string> code, std::vector<Token> tokensVec) : code{std::move(code)},
                                                                              tokens{std::move(tokensVec)},
                                                                              currentToken{tokens.at(0)} {}

std::variant<AstNodePtr, std::vector<Error>>
Parser::Parse() {
  std::vector<AstNodePtr> statements;

  while (!IsFinishedParsing()) {
    statements.push_back(ParseFunctionDeclaration() ?: ParseStatement());
  }

  if (errors.empty()) {
    Token dummyToken{TK_OPEN_CURLY, "", {"", 0, 0}};
    return std::make_shared<Block>(dummyToken, statements);
  } else {
    return errors;
  }
}

AstNodePtr
Parser::ParseFunctionDeclaration() {
  if (std::unique_ptr<Token> defToken = Consume(TK_DEF)) {
    std::unique_ptr<Token> identifierToken =
        Expect(TK_IDENTIFIER, ErrorFactory::Expected(TK_IDENTIFIER, currentToken, code));

    Expect(TK_OPEN_PAREN, ErrorFactory::Expected(TK_OPEN_PAREN, currentToken, code));
    std::vector<AstNodePtr> parameters = ParseFunctionParameters();
    Expect(TK_CLOSED_PAREN, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));

    Expect(TK_ARROW, ErrorFactory::Expected(TK_ARROW, currentToken, code));
    std::unique_ptr<Token> datatypeToken = ExpectDataType();

    AstNodePtr body = ParseFunctionDeclarationBody();

    std::string identifier = identifierToken ? identifierToken->GetStr() : "";
    TypePtr datatype = datatypeToken ? TokenTypeToDataType(datatypeToken->GetTokenType()) : nullptr;

    return std::make_shared<FunctionDeclaration>(*defToken, identifier, parameters, datatype, body);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionParameters() {
  std::vector<AstNodePtr> parameters{};

  do {
    if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));
      TypePtr dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());

      AstNodePtr parameter = std::make_shared<FunctionParameter>(*identifier, identifier->GetStr(), dataType);
      parameters.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  return parameters;
}

AstNodePtr
Parser::ParseFunctionDeclarationBody() {
  std::vector<AstNodePtr> statements{};

  if (std::unique_ptr<Token> curlyToken = Expect(TK_OPEN_CURLY, ErrorFactory::Expected(TK_OPEN_CURLY, currentToken, code))) {
    while (!Peek(0, TK_CLOSED_CURLY)) {
      statements.push_back(ParseStatement());
    }

    Expect(TK_CLOSED_CURLY, ErrorFactory::Expected(TK_CLOSED_CURLY, currentToken, code));

    return std::make_shared<Block>(*curlyToken, statements);
  } else {
    Recover(TK_CLOSED_CURLY);

    return std::make_shared<Block>(currentToken, statements);
  }
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
  if (std::unique_ptr<Token> retToken = Consume(TK_RET)) {
    AstNodePtr returnValue{ParseEqualityExpression()};
    Consume(TK_SEMICOLON);

    return std::make_shared<ReturnStatement>(*retToken, returnValue);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseBlock() {
  if (std::unique_ptr<Token> curlyToken = Consume(TK_OPEN_CURLY)) {
    std::vector<AstNodePtr> statements;

    while (!Consume(TK_CLOSED_CURLY)) {
      statements.push_back(ParseStatement());
    }

    return std::make_shared<Block>(*curlyToken, statements);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseIfElseStatement() {
  if (std::unique_ptr<Token> ifToken = Consume(TK_IF)) {
    Expect(TK_OPEN_PAREN, ErrorFactory::Expected(TK_OPEN_PAREN, currentToken, code));
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));
    AstNodePtr consequent{ParseStatement()};

    if (Consume(TK_ELSE)) {
      AstNodePtr alternative{ParseStatement()};
      return std::make_shared<IfElseStatement>(*ifToken, condition, consequent, alternative);
    }

    return std::make_shared<IfStatement>(*ifToken, condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseWhileLoop() {
  if (std::unique_ptr<Token> whileToken = Consume(TK_WHILE)) {
    Expect(TK_OPEN_PAREN, ErrorFactory::Expected(TK_OPEN_PAREN, currentToken, code));
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));
    AstNodePtr consequent{ParseStatement()};

    return std::make_shared<WhileLoop>(*whileToken, condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseDoWhileLoop() {
  if (std::unique_ptr<Token> doToken = Consume(TK_DO)) {
    AstNodePtr consequent{ParseStatement()};
    Expect(TK_WHILE, ErrorFactory::Expected(TK_OPEN_PAREN, currentToken, code));
    Expect(TK_OPEN_PAREN, ErrorFactory::Expected(TK_OPEN_PAREN, currentToken, code));
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));

    return std::make_shared<DoWhileLoop>(*doToken, condition, consequent);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseExpression() {
  if (AstNodePtr assignmentNode = ParseAssignment()) {
    return assignmentNode;
  } else if (AstNodePtr reassignmentNode = ParseReassignment()) {
    return reassignmentNode;
  } else if (AstNodePtr equalityNode = ParseEqualityExpression()) {
    return equalityNode;
  }

  return nullptr;
}

AstNodePtr
Parser::ParseAssignment() {
  if (std::unique_ptr<Token> assignmentToken = ConsumeOneOf({TK_LET, TK_VAL})) {
    std::string identifier = Consume(TK_IDENTIFIER)->GetStr();

    Expect(TK_COLON, ErrorFactory::Expected(TK_COLON, currentToken, code));

    TypePtr dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());
    dataType->SetMutability(assignmentToken->GetTokenType() == TK_LET);

    Expect(TK_ASSIGN, ErrorFactory::Expected(TK_ASSIGN, currentToken, code));

    return std::make_shared<AssignmentExpression>(*assignmentToken,
                                                  identifier,
                                                  dataType,
                                                  ParseEqualityExpression());
  }


  return nullptr;
}

AstNodePtr
Parser::ParseReassignment() {
  if (Peek(1, TK_ASSIGN)) {
    std::unique_ptr<Token> identifierToken = Consume(TK_IDENTIFIER);
    Expect(TK_ASSIGN, ErrorFactory::Expected(TK_ASSIGN, currentToken, code));
    return std::make_shared<ReassignmentExpression>(*identifierToken,
                                                    identifierToken->GetStr(),
                                                    ParseEqualityExpression());
  }

  return nullptr;
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

    return std::make_shared<BinaryOperation>(*operatorToken, expression, ParseAddExpression());
  }

  return expression;
}

AstNodePtr
Parser::ParseAddExpression() {
  AstNodePtr expression = ParseMulExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_PLUS, TK_MINUS})) {
    return std::make_shared<BinaryOperation>(*operatorToken, expression, ParseMulExpression());
  }

  return expression;
}

AstNodePtr
Parser::ParseMulExpression() {
  AstNodePtr expression = ParsePrimaryExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_STAR, TK_SLASH, TK_PERCENT})) {
    return std::make_shared<BinaryOperation>(*operatorToken, expression, ParsePrimaryExpression());
  }

  return expression;
}

AstNodePtr
Parser::ParsePrimaryExpression() {
  if (AstNodePtr stringNode = ParseString()) {
    return stringNode;
  } else if (AstNodePtr numberNode = ParseNumber()) {
    return numberNode;
  } else if (Peek(0, TK_EXTERN) || Peek(1, TK_OPEN_PAREN)) {
    return ParseFunctionCall();
  } else if (AstNodePtr identifierNode = ParseIdentifier()) {
    return identifierNode;
  } else if (AstNodePtr boolNode = ParseBool()) {
    return boolNode;
  } else if (std::unique_ptr<Token> nullToken = Consume(TK_NULL)) {
    return std::make_shared<NullValue>(*nullToken);
  }

  return nullptr;
}

AstNodePtr
Parser::ParseFunctionCall() {
  bool isExtern = static_cast<bool>(Consume(TK_EXTERN));
  std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER);

  if (Consume(TK_OPEN_PAREN)) {
    std::vector<AstNodePtr> arguments = ParseFunctionArguments();
    Expect(TK_CLOSED_PAREN, ErrorFactory::Expected(TK_CLOSED_PAREN, currentToken, code));

    return std::make_shared<FunctionCall>(*identifier, identifier->GetStr(), arguments, isExtern);
  }

  return nullptr;
}

std::vector<AstNodePtr>
Parser::ParseFunctionArguments() {
  std::vector<AstNodePtr> arguments{};

  do {
    if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON, ErrorFactory::Expected(TK_COLON, currentToken, code));
      AstNodePtr value = ParseEqualityExpression();

      AstNodePtr parameter = std::make_shared<FunctionArgument>(*identifier, identifier->GetStr(), value);
      arguments.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  // If there are no more arguments, generate an error if the next token isn't a closing parenthesis
  PeekWithError(0, TK_CLOSED_PAREN, ErrorFactory::Expected("expected value or identifier", currentToken, code));

  return arguments;
}

AstNodePtr
Parser::ParseNumber() {
  if (std::shared_ptr<Token> integerPortion = Consume(TK_NUMBER)) {
    if (Consume(TK_DOT)) { // Float value
      std::shared_ptr<Token> decimalPortion = Consume(TK_NUMBER);

      return std::make_shared<DecimalLiteral>(*integerPortion, integerPortion->GetStr(), decimalPortion->GetStr());
    } // Integer value
    else {
      return std::make_shared<IntegerLiteral>(*integerPortion, integerPortion->GetStr());
    }
  } else {
    return nullptr;
  }
}

AstNodePtr
Parser::ParseString() {
  if (Consume(TK_QUOTATION)) {
    std::unique_ptr<Token> stringToken = Consume(TK_STRING);
    Expect(TK_QUOTATION, ErrorFactory::Expected("unterminated string", currentToken, code));

    return std::make_shared<StringLiteral>(*stringToken, stringToken->GetStr());
  }

  return nullptr;
}

AstNodePtr
Parser::ParseIdentifier() {
  if (std::shared_ptr<Token> identifierNode = Consume(TK_IDENTIFIER)) {
    Variable numberLiteralNode{*identifierNode, identifierNode->GetStr()};
    return std::make_shared<Variable>(numberLiteralNode);
  } else {
    return nullptr;
  }
}

AstNodePtr
Parser::ParseBool() {
  if (std::shared_ptr<Token> trueToken = Consume(TK_TRUE)) {
    return std::make_shared<BoolValue>(*trueToken, true);
  } else if (std::shared_ptr<Token> falseToken = Consume(TK_FALSE)) {
    return std::make_shared<BoolValue>(*falseToken, false);
  }

  return nullptr;
}

std::unique_ptr<Token>
Parser::Consume(TokenType tokenType) {
  if (currentToken.GetTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    Advance();

    return consumedToken;
  } else {
    return nullptr;
  }
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

std::unique_ptr<Token>
Parser::Expect(TokenType tokenType, const Error error) {
  if (currentToken.GetTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    Advance();

    return consumedToken;
  } else {
    //ReportError(errorMessage, currentToken.GetLocation());
    errors.push_back(error);

    return nullptr;
  }
}

std::unique_ptr<Token>
Parser::ExpectOneOf(const std::list<TokenType>& possibleTokenTypes, const std::string& errorMessage) {
  for (const TokenType& tokenType : possibleTokenTypes) {
    std::unique_ptr<Token> consumedToken = Consume(tokenType);
    if (consumedToken) {
      return consumedToken;
    }
  }

  ReportError(errorMessage, currentToken.GetLocation());

  return nullptr;
}

std::unique_ptr<Token>
Parser::ExpectDataType() {
  return ExpectOneOf({TK_BOOL,
                      TK_U8,
                      TK_U16,
                      TK_U32,
                      TK_U64,
                      TK_U128,
                      TK_I8,
                      TK_I16,
                      TK_I32,
                      TK_I64,
                      TK_I128,
                      TK_F32,
                      TK_F64,
                      TK_STRING},
                     "expected datatype");
}

bool
Parser::Peek(size_t lookaheadDistance, TokenType tokenType) {
  //TODO: Add bound check
  return tokens.at(currentTokenIndex + lookaheadDistance).GetTokenType() == tokenType;
}

bool
Parser::PeekWithError(size_t lookaheadDistance, TokenType tokenType, const Error error) {
  if (Peek(lookaheadDistance, tokenType)) {
    return true;
  } else {
    //ReportError(errorMessage, currentToken.GetLocation());
    errors.push_back(error);

    return false;
  }
}

void
Parser::ReportError(const std::string& errorMessage, const Location& location) {
  std::string codeLine = code.at(currentToken.GetLocation().lineNumber);
  Location errorLocation = currentToken.GetLocation();
  errorLocation.characterLineIndex = errorLocation.characterLineIndex - 1;
  Error error{"syntax error", errorMessage, errorLocation, codeLine};
  errors.push_back(error);
}

void
Parser::Recover(TokenType synchronizationToken) {
  while (currentToken.GetTokenType() != synchronizationToken && !IsFinishedParsing()) {
    Advance();
  }

  if(!IsFinishedParsing()) {
    Advance();
  }
}

void
Parser::Advance() {
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