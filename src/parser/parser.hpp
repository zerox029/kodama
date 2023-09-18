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
  explicit Parser(std::vector<Token> tokens);
  AstNodePtr Parse();

 private:
  std::vector<Token> tokens;
  size_t currentTokenIndex;
  Token currentToken;

  AstNodePtr ParseFunctionDeclaration();
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

  std::unique_ptr<Token> Consume(TokenType tokenType);
  std::unique_ptr<Token> ConsumeOneOf(const std::list<TokenType>& possibleTokenTypes);
  std::unique_ptr<Token> ConsumeDataType();
  void Expect(TokenType tokenType);
  void advance();

  bool IsFinishedParsing();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
