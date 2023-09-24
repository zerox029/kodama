//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_PARSER_PARSER_HPP_
#define KODAMA_SRC_PARSER_PARSER_HPP_

#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include "../errors/error.hpp"
#include <queue>
#include <utility>
#include <memory>
#include <list>

class Parser {
 public:
  explicit Parser(std::vector<std::string> code, std::vector<Token> tokens);
  std::variant<AstNodePtr, std::vector<Error>> Parse();

 private:
  std::vector<std::string> code;
  std::vector<Token> tokens;
  size_t currentTokenIndex = 0;
  Token currentToken;

  std::vector<Error> errors;

  std::unordered_map<std::string, TypePtr> symbolTable;

  AstNodePtr ParseFunctionDeclaration();
  std::vector<AstNodePtr> ParseFunctionParameters();
  AstNodePtr ParseFunctionDeclarationBody();
  AstNodePtr ParseStatement();
  AstNodePtr ParseIfElseStatement();
  AstNodePtr ParseBlock();
  AstNodePtr ParseReturn();
  AstNodePtr ParseWhileLoop();
  AstNodePtr ParseDoWhileLoop();
  AstNodePtr ParseExpression();
  AstNodePtr ParseAssignment();
  AstNodePtr ParseReassignment();
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

  /**
   * Consumes the current token and returns it if it is of the specified type. Returns a nullptr otherwise.
   * @param tokenType The token to match
   * @return The matched token or a nullptr
   */
  std::unique_ptr<Token> Consume(TokenType tokenType);
  std::unique_ptr<Token> ConsumeOneOf(const std::list<TokenType>& possibleTokenTypes);

  /**
   * Consumes the current token and returns it if it is of the specified type, reports and error otherwise.
   * @param tokenType The token to match
   * @param errorMessage The error message to display if the token is not found
   * @return The matched token or a nullptr
   */
  std::unique_ptr<Token> Expect(TokenType tokenType, const std::string& errorMessage);
  std::unique_ptr<Token> ExpectOneOf(const std::list<TokenType>& possibleTokenTypes, const std::string& errorMessage);
  std::unique_ptr<Token> ExpectDataType();


  /**
   * Checks if the specified token is of the specified type.
   * @param lookaheadDistance How far ahead in the list to look for the token
   * @param tokenType The token to match
   * @return Whether or not the token was found
   */
  bool Peek(size_t lookaheadDistance, TokenType tokenType);

  /**
   * Checks if the specified token is of the specified type, reports an error otherwise.
   * @param lookaheadDistance How far ahead in the list to look for the token
   * @param tokenType The token to match
   * @param errorMessage The error message to display if the token is not found
   * @return
   */
  bool PeekWithError(size_t lookaheadDistance, TokenType tokenType, const std::string& errorMessage);

  /**
   * Adds the specified error to the error list
   */
  void ReportError(const std::string& errorMessage, const Location& location);

  /**
   * Panic-mode recovery
   * When an error is found, skip tokens until a recovery token is hit.
   * Ideally, update this to be more context based and/or add phrase-level recovery
   */
  void Recover(TokenType synchronizationToken);

  void Advance();
  bool IsFinishedParsing();
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_