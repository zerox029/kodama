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

std::variant<AstNodePtr, std::vector<Errors::Error>>
Parser::Parse() {
  std::vector<AstNodePtr> statements;

  while (!IsFinishedParsing()) {
    statements.push_back(ParseFunctionDeclaration() ?: ParseStatement());
  }

  if (errors.empty()) {
    Token dummyToken{TK_OPEN_CURLY, "", {"", 0, 0}};
    return std::make_shared<Program>(dummyToken, statements);
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
          Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, currentToken.GetStr());

      Expect(TK_OPEN_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());
      std::vector<AstNodePtr> parameters = ParseFunctionParameters();
      Expect(TK_CLOSED_PAREN, Errors::EXPECTED_CL_DELIMITER, std::string(")"), currentToken.GetStr());

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

        AstNodePtr parameter = std::make_shared<Parameter>(*identifier, identifier->GetStr(), dataType);
        parameters.push_back(parameter);
      }
    } while (Consume(TK_COMMA));

    return parameters;
  }
  catch (const ParsingException& parsingException) {
    RecoverKeepSynchronizationToken(TK_CLOSED_PAREN);
    return parameters;
  }
}

/// function_body : '{' statement '}' | '=>' equality;
AstNodePtr
Parser::ParseFunctionBody() {
  std::vector<AstNodePtr> statements{};

  std::unique_ptr<Token> openingToken{};
  try {
    openingToken = ExpectOneOf({TK_OPEN_CURLY, TK_DOUBLE_ARROW}, Errors::EXPECTED_FUNCTION_BODY, currentToken.GetStr());

    if (openingToken->GetTokenType() == TK_OPEN_CURLY) {
      while (!Peek(0, TK_CLOSED_CURLY)) {
        statements.push_back(ParseStatement());
      }

      Expect(TK_CLOSED_CURLY, Errors::EXPECTED_CL_DELIMITER, std::string("}"), currentToken.GetStr());

      return std::make_shared<Block>(*openingToken, statements);
    } else {
      AstNodePtr returnValue = ParseEqualityExpression();

      return std::make_shared<ReturnStatement>(*openingToken, returnValue);
    }
  }
  catch (const ParsingException& parsingException) {
    Token errorToken = openingToken ? *openingToken : currentToken;

    Recover(TK_CLOSED_CURLY);

    return std::make_shared<Block>(errorToken, statements);
  }
}

/// statement: return | block | if_else | while | do_while | expression ';'
AstNodePtr
Parser::ParseStatement() {
  try {
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
    } else if (AstNodePtr forLoop = ParseForLoop()) {
      return forLoop;
    } else if(AstNodePtr structNode = ParseStructDefinition()) {
      return structNode;
    } else {
      AstNodePtr expression = ParseExpression();
      ExpectOneOf({TK_SEMICOLON, TK_NEW_LINE}, Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());

      return expression;
    }
  }
  catch (const ParsingException& parsingException) {
    Token errorToken = currentToken;

    RecoverWithOneOf({TK_SEMICOLON, TK_NEW_LINE});

    return std::make_shared<NullValue>(errorToken);
  }
}

/// struct : 'struct' identifier '{' [ structMemberDefinition ] '}'
AstNodePtr
Parser::ParseStructDefinition() {
  if(std::unique_ptr<Token> structToken = Consume(TK_STRUCT)) {
    std::unique_ptr<Token> identifierToken =
        Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, currentToken.GetStr());

    Expect(TK_OPEN_CURLY, Errors::EXPECTED_OP_DELIMITER, std::string("{"), currentToken.GetStr());

    std::vector<AstNodePtr> structMembers{};
    while(!Peek(0, TK_CLOSED_CURLY)) {
      AstNodePtr member = ParseStructMemberDefinition();
      if(member) {
        structMembers.push_back(member);
      }
    }

    Expect(TK_CLOSED_CURLY, Errors::EXPECTED_CL_DELIMITER, std::string("}"), currentToken.GetStr());

    std::string identifier = identifierToken ? identifierToken->GetStr() : "";
    return std::make_shared<StructDefinition>(*structToken, identifier, structMembers);
  }

  return nullptr;
}

/// structMemberDefinition : identifier ':' data_type (';' | '\n')
AstNodePtr
Parser::ParseStructMemberDefinition() {
  std::unique_ptr<Token> identifierToken =
      Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, currentToken.GetStr());

  Expect(TK_COLON, Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());

  TypePtr dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());

  ExpectOneOf({TK_SEMICOLON, TK_NEW_LINE}, Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());

  std::string identifier = identifierToken ? identifierToken->GetStr() : "";
  return std::make_shared<Parameter>(*identifierToken, identifier, dataType);
}

/// structInit : identifier '{' [ structMemberInit ] '}'
AstNodePtr
Parser::ParseStructInit() {
  if(std::unique_ptr<Token> identifierToken = Consume(TK_IDENTIFIER)) {
    Expect(TK_OPEN_CURLY, Errors::EXPECTED_OP_DELIMITER, std::string("{"), currentToken.GetStr());

    std::vector<AstNodePtr> structMembers{};
    while(!Peek(0, TK_CLOSED_CURLY)) {
      AstNodePtr member = ParseStructMemberInit();
      if(member) {
        structMembers.push_back(member);
      }
    }

    Expect(TK_CLOSED_CURLY, Errors::EXPECTED_CL_DELIMITER, std::string("}"), currentToken.GetStr());

    std::string identifier = identifierToken ? identifierToken->GetStr() : "";
    return std::make_shared<StructInit>(*identifierToken, identifier, structMembers);
  }

  return nullptr;
}

/// structMemberInit : identifier ':' equality (';' | '\n')
AstNodePtr
Parser::ParseStructMemberInit() {
  std::unique_ptr<Token> identifierToken =
      Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, currentToken.GetStr());

  Expect(TK_COLON, Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());

  AstNodePtr value = ParseEqualityExpression();
  if(!value) {
    LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
  }

  ExpectOneOf({TK_SEMICOLON, TK_NEW_LINE}, Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());

  std::string identifier = identifierToken ? identifierToken->GetStr() : "";
  return std::make_shared<Argument>(*identifierToken, identifier, value);
}

/// return: 'return' expression [';'];
AstNodePtr
Parser::ParseReturn() {
  if (std::unique_ptr<Token> retToken = Consume(TK_RET)) {
    AstNodePtr returnValue{ParseEqualityExpression()};

    ExpectOneOf({TK_SEMICOLON, TK_NEW_LINE}, Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());

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
    Expect(TK_OPEN_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());

    AstNodePtr condition{ParseEqualityExpression()};
    if (!condition) {
      LogError(Errors::EXPECTED_CONDITION);
    }

    Expect(TK_CLOSED_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string(")"), currentToken.GetStr());
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
    if (!condition) {
      LogError(Errors::EXPECTED_CONDITION);
    }

    Expect(TK_CLOSED_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string(")"), currentToken.GetStr());
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
    Expect(TK_WHILE, Errors::EXPECTED_KEYWORD, std::string("while"), currentToken.GetStr());
    Expect(TK_OPEN_PAREN, Errors::EXPECTED_OP_DELIMITER, std::string("("), currentToken.GetStr());

    AstNodePtr condition{ParseEqualityExpression()};
    if (!condition) {
      LogError(Errors::EXPECTED_CONDITION);
    }

    Expect(TK_CLOSED_PAREN, Errors::EXPECTED_CL_DELIMITER, std::string(")"), currentToken.GetStr());

    return std::make_shared<DoWhileLoop>(*doToken, condition, consequent);
  }

  return nullptr;
}

/// forLoop: 'for' identifier 'in' add_expression ('to' | 'until') add_expression statement
AstNodePtr
Parser::ParseForLoop() {
  if (std::unique_ptr<Token> forToken = Consume(TK_FOR)) {
    std::unique_ptr<Token> identifierToken = Expect(TK_IDENTIFIER, Errors::EXPECTED_IDENTIFIER, currentToken.GetStr());
    Expect(TK_IN, Errors::EXPECTED_KEYWORD, currentToken.GetStr());

    AstNodePtr from = ParseAddExpression();
    if (!from) {
      LogError(Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());
    }

    std::unique_ptr<Token>
        toUntilToken = ExpectOneOf({TK_TO, TK_UNTIL}, Errors::EXPECTED_TO_UNTIL, currentToken.GetStr());

    AstNodePtr to = ParseAddExpression();
    if (!to) {
      LogError(Errors::UNEXPECTED_EXPRESSION, currentToken.GetStr());
    }

    AstNodePtr consequent{ParseStatement()};

    if (toUntilToken->GetTokenType() == TK_UNTIL) {
      Token subtractionToken = Token{TK_MINUS, "-", {"", 0, 0}};
      AstNodePtr subOne = std::make_shared<IntegerLiteral>(*toUntilToken, "1");
      to = std::make_shared<BinaryOperation>(subtractionToken, to, subOne);
    }

    return std::make_shared<ForLoop>(*forToken, identifierToken->GetStr(), from, to, consequent);
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

///assignment: ('let' | 'val') identifier ':' data_type '=' (structInit | equality)
AstNodePtr
Parser::ParseAssignment() {
  if (std::unique_ptr<Token> assignmentToken = ConsumeOneOf({TK_LET, TK_VAL})) {
    std::string identifier = Consume(TK_IDENTIFIER)->GetStr();

    Expect(TK_COLON, Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());

    TypePtr dataType{};
    if(std::unique_ptr<Token> userType = Consume(TK_IDENTIFIER)) {
      dataType = std::make_shared<StructType>(userType->GetStr());
    }
    else {
      dataType = TokenTypeToDataType(ExpectDataType()->GetTokenType());
    }

    dataType->SetMutability(assignmentToken->GetTokenType() == TK_LET);

    if (Consume(TK_ASSIGN)) {
      AstNodePtr rhs = ParseStructInit() ?: ParseEqualityExpression();
      if (!rhs) {
        LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
      }

      return std::make_shared<AssignmentExpression>(*assignmentToken, identifier, dataType, rhs);
    }

    return std::make_shared<AssignmentExpression>(*assignmentToken, identifier, dataType);
  }

  return nullptr;
}

///reassignment: identifier '=' equality
AstNodePtr
Parser::ParseReassignment() {
  if (PeekOneOf(1, {TK_ASSIGN, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_STAR_ASSIGN, TK_SLASH_ASSIGN, TK_MOD_ASSIGN})) {
    std::unique_ptr<Token> identifierToken = Consume(TK_IDENTIFIER);

    std::unique_ptr<Token> assignmentToken =
        ExpectOneOf({TK_ASSIGN, TK_PLUS_ASSIGN, TK_MINUS_ASSIGN, TK_STAR_ASSIGN, TK_SLASH_ASSIGN, TK_MOD_ASSIGN},
                    Errors::UNEXPECTED_TOKEN,
                    currentToken.GetStr());

    AstNodePtr rhs = ParseEqualityExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    // Handle arithmetic assignments
    if(assignmentToken->GetTokenType() == TK_PLUS_ASSIGN) {
      Token operatorToken{TK_PLUS, "+", {assignmentToken->GetLocation()}};
      AstNodePtr lhs = std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
      rhs = std::make_shared<BinaryOperation>(operatorToken, lhs, rhs);
    }
    else if(assignmentToken->GetTokenType() == TK_MINUS_ASSIGN) {
      Token operatorToken{TK_MINUS, "-", {assignmentToken->GetLocation()}};
      AstNodePtr lhs = std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
      rhs = std::make_shared<BinaryOperation>(operatorToken, lhs, rhs);
    }
    else if(assignmentToken->GetTokenType() == TK_STAR_ASSIGN) {
      Token operatorToken{TK_STAR, "*", {assignmentToken->GetLocation()}};
      AstNodePtr lhs = std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
      rhs = std::make_shared<BinaryOperation>(operatorToken, lhs, rhs);
    }
    else if(assignmentToken->GetTokenType() == TK_SLASH_ASSIGN) {
      Token operatorToken{TK_SLASH_ASSIGN, "/", {assignmentToken->GetLocation()}};
      AstNodePtr lhs = std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
      rhs = std::make_shared<BinaryOperation>(operatorToken, lhs, rhs);
    }
    else if(assignmentToken->GetTokenType() == TK_MOD_ASSIGN) {
      Token operatorToken{TK_PERCENT, "%", {assignmentToken->GetLocation()}};
      AstNodePtr lhs = std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
      rhs = std::make_shared<BinaryOperation>(operatorToken, lhs, rhs);
    }

    return std::make_shared<ReassignmentExpression>(*identifierToken, identifierToken->GetStr(), rhs);
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
    if (!expression) { // Missing lhs
      LogErrorAtPosition(Errors::UNEXPECTED_TOKEN, operatorToken->GetLocation(), operatorToken->GetStr());
    }

    AstNodePtr rhs = ParseEqualityExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    return std::make_shared<BinaryOperation>(*operatorToken, expression, rhs);
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
    AstNodePtr rhs = ParseAddExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    return std::make_shared<BinaryOperation>(*operatorToken, expression, rhs);
  }

  return expression;
}

/// mul_expression: unary '*' mul_expression
///               | unary '/' mul_expression
///               | unary '%' mul_expression
///               | unary
AstNodePtr
Parser::ParseMulExpression() {
  AstNodePtr expression = ParseUnaryExpression();

  if (std::shared_ptr<Token> operatorToken = ConsumeOneOf({TK_STAR, TK_SLASH, TK_PERCENT})) {
    if (!expression && operatorToken->GetStr() != "*") { // Missing lhs
      LogErrorAtPosition(Errors::UNEXPECTED_TOKEN, operatorToken->GetLocation(), operatorToken->GetStr());
    }

    AstNodePtr rhs = ParseMulExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    return std::make_shared<BinaryOperation>(*operatorToken, expression, rhs);
  }

  return expression;
}

/// unary: ('-' | '+')? primary
AstNodePtr
Parser::ParseUnaryExpression() {
  if (std::shared_ptr<Token> plusToken = Consume(TK_PLUS)) {
    AstNodePtr rhs = ParsePrimaryExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    return rhs;
  } else if (std::shared_ptr<Token> minusToken = Consume(TK_MINUS)) {
    AstNodePtr lhs = std::make_shared<IntegerLiteral>(*minusToken, "0");

    AstNodePtr rhs = ParsePrimaryExpression();
    if (!rhs) {
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
    }

    return std::make_shared<BinaryOperation>(*minusToken, lhs, rhs);
  }

  return ParsePrimaryExpression();
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
    Expect(TK_CLOSED_PAREN, Errors::EXPECTED_CL_DELIMITER, std::string(")"), currentToken.GetStr());

    return std::make_shared<FunctionCall>(*identifier, identifier->GetStr(), arguments, isExtern);
  }

  return nullptr;
}

/// function_arguments: { [identifier ':'] equality ',' } [identifier ':'] equality
std::vector<AstNodePtr>
Parser::ParseFunctionArguments() {
  std::vector<AstNodePtr> arguments{};

  if (Peek(0, TK_CLOSED_PAREN)) {
    return arguments;
  }

  do {
    if (Peek(1, TK_COLON)) {
      std::unique_ptr<Token> identifier = Consume(TK_IDENTIFIER);
      Expect(TK_COLON, Errors::EXPECTED_TOKEN, std::string(":"), currentToken.GetStr());

      AstNodePtr value = ParseEqualityExpression();
      if (!value) {
        LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
        throw ParsingException();
      }

      AstNodePtr parameter = std::make_shared<Argument>(*identifier, identifier->GetStr(), value);
      arguments.push_back(parameter);
    } else if (Peek(0, TK_CLOSED_PAREN)) { // Trailing comma
      LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
      throw ParsingException();
    } else {
      AstNodePtr value = ParseEqualityExpression();
      if (!value) {
        LogError(Errors::EXPECTED_VALUE_IDENTIFIER);
        throw ParsingException();
      }

      AstNodePtr parameter = std::make_shared<Argument>(value->GetToken(), value);
      arguments.push_back(parameter);
    }
  } while (Consume(TK_COMMA));

  // If there are no more arguments, generate an error if the next token isn't a closing parenthesis
  if (!PeekWithError(0, TK_CLOSED_PAREN, Errors::UNEXPECTED_TOKEN, currentToken.GetStr())) {
    throw ParsingException();
  }

  return arguments;
}

/// '-'? ('0'-'9')+
AstNodePtr
Parser::ParseNumber() {
  bool isNegative = Consume(TK_MINUS) == nullptr;
  if (std::shared_ptr<Token> integerPortion = Consume(TK_NUMBER)) {
    if (Consume(TK_DOT)) { // Float value
      std::shared_ptr<Token> decimalPortion = Consume(TK_NUMBER);

      return std::make_shared<DecimalLiteral>(*integerPortion, integerPortion->GetStr(), decimalPortion->GetStr());
    }

    // Integer value
    return std::make_shared<IntegerLiteral>(*integerPortion, integerPortion->GetStr());
  }

  return nullptr;
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
  if (std::shared_ptr<Token> identifierToken = Consume(TK_IDENTIFIER)) {
    return std::make_shared<Variable>(*identifierToken, identifierToken->GetStr());
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
  if (tokenType == TK_NEW_LINE && currentToken.GetTokenType() == TK_NEW_LINE) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    Advance();

    return consumedToken;
  } else {
    size_t newlineCount = GetNewLineCountFromCurrentToken();

    if (tokens.at(currentTokenIndex + newlineCount).GetTokenType() == tokenType) {
      IgnoreNewlines();
      std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
      Advance();

      return consumedToken;
    } else {
      return nullptr;
    }
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
  if (tokenType == TK_NEW_LINE && currentToken.GetTokenType() == TK_NEW_LINE) {
    std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
    Advance();

    return consumedToken;
  } else {
    size_t newlineCount = GetNewLineCountFromCurrentToken();

    if (tokens.at(currentTokenIndex + newlineCount).GetTokenType() == tokenType) {
      IgnoreNewlines();
      std::unique_ptr<Token> consumedToken = std::make_unique<Token>(currentToken);
      Advance();

      return consumedToken;
    }

    LogError(errorType, std::forward<T>(args)...);

    throw ParsingException();
  }
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

  LogError(errorType, std::forward<T>(args)...);

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
  lookaheadDistance += GetNewLineCountFromCurrentToken();

  //TODO: Add bound check
  return tokens.at(currentTokenIndex + lookaheadDistance).GetTokenType() == tokenType;
}

bool Parser::PeekOneOf(size_t lookaheadDistance, const std::list<TokenType>& possibleTokenTypes) {
  lookaheadDistance += GetNewLineCountFromCurrentToken();

  for (const TokenType& tokenType : possibleTokenTypes) {
    //TODO: Add bound check
    if(tokens.at(currentTokenIndex + lookaheadDistance).GetTokenType() == tokenType) {
      return true;
    }
  }

  return false;
}

template<class... T>
bool
Parser::PeekWithError(size_t lookaheadDistance, TokenType tokenType, Errors::ErrorType errorType, T&& ... args) {
  if (Peek(lookaheadDistance, tokenType)) {
    return true;
  } else {
    LogError(errorType, std::forward<T>(args)...);

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
Parser::RecoverKeepSynchronizationToken(TokenType synchronizationToken) {
  while (currentToken.GetTokenType() != synchronizationToken && !IsFinishedParsing()) {
    Advance();
  }
}

void
Parser::RecoverWithOneOf(const std::set<TokenType>& synchronizationTokens) {
  while (!synchronizationTokens.contains(currentToken.GetTokenType()) && !IsFinishedParsing()) {
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

void
Parser::IgnoreNewlines() {
  while (currentToken.GetTokenType() == TK_NEW_LINE) {
    Advance();
  }
}

size_t
Parser::GetNewLineCountFromCurrentToken() {
  size_t localIndex = currentTokenIndex;
  while (tokens.at(localIndex).GetTokenType() == TK_NEW_LINE) {
    localIndex++;
  }

  return localIndex - currentTokenIndex;
}

template<class... T>
void
Parser::LogError(Errors::ErrorType errorType, T&& ... args) {
  errors.push_back(Errors::Generate(errorType, currentToken.GetLocation(), code, std::forward<T>(args)...));
}

template<class... T>
void
Parser::LogErrorAtPosition(Errors::ErrorType errorType, Location location, T&& ... args) {
  errors.push_back(Errors::Generate(errorType, location, code, std::forward<T>(args)...));
}