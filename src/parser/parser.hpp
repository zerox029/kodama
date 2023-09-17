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
  std::shared_ptr<AstNode> Parse();

 private:
  std::vector<Token> tokens;
  size_t currentTokenIndex;
  Token currentToken;

  std::shared_ptr<AstNode> ParseStatement();
  std::shared_ptr<AstNode> ParseExpression();
  std::shared_ptr<AstNode> ParseAssignment();
  std::shared_ptr<AstNode> ParseEqualityExpression();
  std::shared_ptr<AstNode> ParseAddExpression();
  std::shared_ptr<AstNode> ParseMulExpression();
  std::shared_ptr<AstNode> ParseNumber();

  std::unique_ptr<Token> Consume(TokenType tokenType);
  std::unique_ptr<Token> ConsumeOneOf(const std::list<TokenType>& possibleTokenTypes);
  std::unique_ptr<Token> ConsumeDataType();
  void Expect(TokenType tokenType);
  void advance();

  bool IsFinishedParsing();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_
