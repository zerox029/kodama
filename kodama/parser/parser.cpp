//
// Created by emma on 12/09/23.
//

#include "parser.hpp"
#include <utility>
#include <iostream>
#include <variant>
#include <functional>
#include <utility>

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

/// function : 'def' identifier '(' function_parameters ')' '->' data_type '{' statement '}'
AstNodePtr
Parser::ParseFunctionDeclaration() {
  if (std::unique_ptr<Token> defToken = Consume(TK_DEF)) {
    try {
      std::unique_ptr<Token> identifierToken =
          Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, std::string("{"));

      Expect(TK_OPEN_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());
      std::vector<AstNodePtr> parameters = ParseFunctionParameters();
      Expect(TK_CLOSED_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());

      Expect(TK_ARROW, Errors::EXPECTED_TOKEN, std::string("->"), currentToken.GetStr());
      std::unique_ptr<Token> datatypeToken = ExpectDataType();

      AstNodePtr body = ParseFunctionBody();

      std::string identifier = identifierToken ? identifierToken->GetStr() : "";
      TypePtr datatype = datatypeToken ? TokenTypeToDataType(datatypeToken->GetTokenType()) : nullptr;

      return std::make_shared<FunctionDeclaration>(*defToken, identifier, parameters, datatype, body);
    }
    catch (const ParsingException& parsingException) {
      Recover(TK_CLOSED_CURLY);

      std::vector<AstNodePtr> dummyParameters{};
      return std::make_shared<FunctionDeclaration>(*defToken, "", dummyParameters, nullptr, nullptr);
    }
  }

  return nullptr;
}

/// function_parameters : { identifier ':' data_type ',' } identifier ':' data_type;
std::vector<AstNodePtr>
Parser::ParseFunctionParameters() {
  std::vector<AstNodePtr> parameters{};

  try {
    do {
      if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
        Expect(TK_COLON, Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());
        TypePtr dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());

        AstNodePtr parameter = std::make_shared<FunctionParameter>(*identifier, identifier->GetStr(), dataType);
        parameters.push_back(parameter);
      }
    } while (Consume(TK_COMMA));

    return parameters;
  }
  catch (const ParsingException& parsingException) {
    Recover(TK_CLOSED_PAREN);
    return parameters;
  }
}

/// function_body : '{' statement '}';
AstNodePtr
Parser::ParseFunctionBody() {
  std::vector<AstNodePtr> statements{};

  std::unique_ptr<Token> curlyToken{};
  try {
    curlyToken = Expect(TK_OPEN_CURLY,  Errors::EXPECTED_OP_DELIMITER, std::string("{"), currentToken.GetStr());

    while (!Peek(0, TK_CLOSED_CURLY)) {
      statements.push_back(ParseStatement());
    }

    Expect(TK_CLOSED_CURLY,  Errors::EXPECTED_OP_DELIMITER, std::string("}"), currentToken.GetStr());

    return std::make_shared<Block>(*curlyToken, statements);
  }
  catch (const ParsingException& parsingException) {
    Token errorToken = curlyToken ? *curlyToken : currentToken;

    Recover(TK_CLOSED_CURLY);

    return std::make_shared<Block>(errorToken, statements);
  }
}

/// statement: return | block | if_else | while | do_while | expression ';'
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

/// return: 'return' expression [';'];
AstNodePtr
Parser::ParseReturn() {
  if (std::unique_ptr<Token> retToken = Consume(TK_RET)) {
    AstNodePtr returnValue{ParseEqualityExpression()};
    Consume(TK_SEMICOLON);

    return std::make_shared<ReturnStatement>(*retToken, returnValue);
  }

  return nullptr;
}

/// block: '{' statement '}'
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

/// if_else: 'if' '(' equality ')' statement ('else' statement)?
AstNodePtr
Parser::ParseIfElseStatement() {
  if (std::unique_ptr<Token> ifToken = Consume(TK_IF)) {
    Expect(TK_OPEN_PAREN,  Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN,  Errors::EXPECTED_OP_DELIMITER, std::string(")"), currentToken.GetStr());
    AstNodePtr consequent{ParseStatement()};

    if (Consume(TK_ELSE)) {
      AstNodePtr alternative{ParseStatement()};
      return std::make_shared<IfElseStatement>(*ifToken, condition, consequent, alternative);
    }

    return std::make_shared<IfStatement>(*ifToken, condition, consequent);
  }

  return nullptr;
}

/// while: 'while' '(' equality ')' statement
AstNodePtr
Parser::ParseWhileLoop() {
  if (std::unique_ptr<Token> whileToken = Consume(TK_WHILE)) {
    Expect(TK_OPEN_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN,  Errors::EXPECTED_OP_DELIMITER, std::string(")"), currentToken.GetStr());
    AstNodePtr consequent{ParseStatement()};

    return std::make_shared<WhileLoop>(*whileToken, condition, consequent);
  }

  return nullptr;
}

/// do_while: 'do' statement 'while' '(' condition ')'
AstNodePtr
Parser::ParseDoWhileLoop() {
  if (std::unique_ptr<Token> doToken = Consume(TK_DO)) {
    AstNodePtr consequent{ParseStatement()};
    Expect(TK_WHILE,  Errors::EXPECTED_KEYWORD, std::string("while"), currentToken.GetStr());
    Expect(TK_OPEN_PAREN,  Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());
    AstNodePtr condition{ParseEqualityExpression()};
    Expect(TK_CLOSED_PAREN,  Errors::EXPECTED_OP_DELIMITER, std::string(")"), currentToken.GetStr());

    return std::make_shared<DoWhileLoop>(*doToken, condition, consequent);
  }

  return nullptr;
}

/// expression: assignment | equality
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

///assignment: ('let' | 'val') identifier ':' data_type '=' equality
AstNodePtr
Parser::ParseAssignment() {
  if (std::unique_ptr<Token> assignmentToken = ConsumeOneOf({TK_LET, TK_VAL})) {
    std::string identifier = Consume(TK_IDENTIFIER)->GetStr();

    Expect(TK_COLON,  Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());

    TypePtr dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());
    dataType->SetMutability(assignmentToken->GetTokenType() == TK_LET);

    Expect(TK_ASSIGN,  Errors::EXPECTED_TOKEN, std::string("="), currentToken.GetStr());

    return std::make_shared<AssignmentExpression>(*assignmentToken,
                                                  identifier,
                                                  dataType,
                                                  ParseEqualityExpression());
  }


  return nullptr;
}

///reassignment: identifier '=' equality
AstNodePtr
Parser::ParseReassignment() {
  if (Peek(1, TK_ASSIGN)) {
    std::unique_ptr<Token> identifierToken = Consume(TK_IDENTIFIER);
    Expect(TK_ASSIGN,  Errors::EXPECTED_TOKEN, std::string("="), currentToken.GetStr());
    return std::make_shared<ReassignmentExpression>(*identifierToken,
                                                    identifierToken->GetStr(),
                                                    ParseEqualityExpression());
  }

  return nullptr;
}

/// equality: add_expression '==' equality
///         | add_expression '!=' equality
///         | add_expression '>' equality
///         | add_expression '<' equality
///         | add_expression '>=' equality
///         | add_expression '<=' equality
///         | add_expression
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

/// add_expression: mul_expression '+' add_expression
///               | mul_expression '-' add_expression
///               | mul_expression
AstNodePtr
Parser::ParseAddExpression() {
  AstNodePtr expression = ParseMulExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_PLUS, TK_MINUS})) {
    return std::make_shared<BinaryOperation>(*operatorToken, expression, ParseMulExpression());
  }

  return expression;
}

/// mul_expression: number '*' mul_expression
///               | number '/' mul_expression
///               | number '%' mul_expression
///               | primary
AstNodePtr
Parser::ParseMulExpression() {
  AstNodePtr expression = ParsePrimaryExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_STAR, TK_SLASH, TK_PERCENT})) {
    return std::make_shared<BinaryOperation>(*operatorToken, expression, ParsePrimaryExpression());
  }

  return expression;
}

/// primary: string | number | call | identifier | bool | 'null'
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

/// call: identifier '(' function_arguments ')'
AstNodePtr
Parser::ParseFunctionCall() {
  bool isExtern = static_cast<bool>(Consume(TK_EXTERN));
  std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER);

  if (Consume(TK_OPEN_PAREN)) {
    std::vector<AstNodePtr> arguments = ParseFunctionArguments();
    Expect(TK_CLOSED_PAREN,  Errors::EXPECTED_CL_DELIMITER, std::string(")"), currentToken.GetStr());

    return std::make_shared<FunctionCall>(*identifier, identifier->GetStr(), arguments, isExtern);
  }

  return nullptr;
}

/// function_arguments: { identifier ':' equality ',' } identifier ':' equality
std::vector<AstNodePtr>
Parser::ParseFunctionArguments() {
  std::vector<AstNodePtr> arguments{};

  do {
    if (std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER)) {
      Expect(TK_COLON,  Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());
      AstNodePtr value = ParseEqualityExpression();

      AstNodePtr parameter = std::make_shared<FunctionArgument>(*identifier, identifier->GetStr(), value);
      arguments.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  // If there are no more arguments, generate an error if the next token isn't a closing parenthesis
  PeekWithError(0, TK_CLOSED_PAREN, Errors::EXPECTED_VALUE_IDENTIFIER);

  return arguments;
}

/// ('0'-'9')+
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

/// string: any string
AstNodePtr
Parser::ParseString() {
  if (Consume(TK_QUOTATION)) {
    std::unique_ptr<Token> stringToken = Consume(TK_STRING);
    Expect(TK_QUOTATION, Errors::UNTERMINATED_STRING);

    return std::make_shared<StringLiteral>(*stringToken, stringToken->GetStr());
  }

  return nullptr;
}

/// {'a'-'z' | 'A'-'Z'}
AstNodePtr
Parser::ParseIdentifier() {
  if (std::shared_ptr<Token> identifierNode = Consume(TK_IDENTIFIER)) {
    Variable numberLiteralNode{*identifierNode, identifierNode->GetStr()};
    return std::make_shared<Variable>(numberLiteralNode);
  } else {
    return nullptr;
  }
}

/// 'true' | 'false'
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

template<class... T>
std::unique_ptr<Token>
Parser::Expect(TokenType tokenType, Errors::ErrorType errorType, T&& ... args) {
  if (currentToken.GetTokenType() == tokenType) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    Advance();

    return consumedToken;
  }

  errors.push_back(Errors::Generate(errorType, currentToken.GetLocation(), code, std::forward<T>(args)...));

  throw ParsingException();
}

template<class... T>
std::unique_ptr<Token>
Parser::ExpectOneOf(const std::list<TokenType>& possibleTokenTypes, Errors::ErrorType errorType, T&& ... args) {
  for (const TokenType& tokenType : possibleTokenTypes) {
    std::unique_ptr<Token> consumedToken = Consume(tokenType);
    if (consumedToken) {
      return consumedToken;
    }
  }

  errors.push_back(Errors::Generate(errorType, currentToken.GetLocation(), code, std::forward<T>(args)...));

  throw ParsingException();
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
                     Errors::EXPECTED_DATATYPE);
}

bool
Parser::Peek(size_t lookaheadDistance, TokenType tokenType) {
  //TODO: Add bound check
  return tokens.at(currentTokenIndex + lookaheadDistance).GetTokenType() == tokenType;
}

template<class... T>
bool
Parser::PeekWithError(size_t lookaheadDistance, TokenType tokenType, Errors::ErrorType errorType, T&& ... args) {
  if (Peek(lookaheadDistance, tokenType)) {
    return true;
  } else {
    errors.push_back(Errors::Generate(errorType, currentToken.GetLocation(), code, std::forward<T>(args)...));

    return false;
  }
}

void
Parser::Recover(TokenType synchronizationToken) {
  while (currentToken.GetTokenType() != synchronizationToken && !IsFinishedParsing()) {
    Advance();
  }

  if (!IsFinishedParsing()) {
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