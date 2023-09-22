//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_PARSER_PARSER_HPP_
#define KODAMA_SRC_PARSER_PARSER_HPP_

#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include <queue>
#include <utility>
#include <memory>
#include <list>

class Parser {
 public:
  explicit Parser(std::vector<std::string> code, std::vector<Token> tokens);
  AstNodePtr Parse();

 private:
  std::vector<std::string> code;
  std::vector<Token> tokens;
  size_t currentTokenIndex = 0;
  Token currentToken;

  AstNodePtr ParseFunctionDeclaration();
  std::vector<AstNodePtr> ParseFunctionParameters();
  AstNodePtr ParseStatement();
  AstNodePtr ParseIfElseStatement();
  AstNodePtr ParseBlock();
  AstNodePtr ParseReturn();
  AstNodePtr ParseWhileLoop();
  AstNodePtr ParseDoWhileLoop();
  AstNodePtr ParseExpression();
  AstNodePtr ParseAssignment();
  AstNodePtr ParseEqualityExpression();
  AstNodePtr ParseAddExpression();
  AstNodePtr ParseMulExpression();
  AstNodePtr ParsePrimaryExpression();
  AstNodePtr ParseFunctionCall();
  std::vector<AstNodePtr> ParseFunctionArguments();
  AstNodePtr ParseIdentifier();
  AstNodePtr ParseNumber();
  AstNodePtr ParseString();
  AstNodePtr ParseBool();

  std::unique_ptr<Token> Consume(TokenType tokenType);
  std::unique_ptr<Token> ConsumeOneOf(const std::list<TokenType>& possibleTokenTypes);
  std::unique_ptr<Token> ConsumeDataType();
  bool LookAhead(size_t lookaheadDistance, TokenType tokenType);
  void Expect(TokenType tokenType, std::string errorMessage);
  void advance();

  bool IsFinishedParsing();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
