//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_PARSER_PARSER_HPP_
#define KODAMA_SRC_PARSER_PARSER_HPP_

#include "../lexer/token.hpp"
#include "../ast/ast.hpp"
#include "../errors/error.hpp"
#include "../errors/errorFactory.hpp"
#include <queue>
#include <utility>
#include <memory>
#include <list>
#include <exception>

class ParsingException : public std::exception {
 public:
  const char* what() const noexcept {
    return "Aborting due to parsing error...";
  }
};

class Parser {
 public:
  explicit Parser(std::vector<std::string> code, std::vector<Token> tokens);
  std::variant<AstNodePtr, std::vector<Errors::Error>> Parse();

 private:
  std::vector<std::string> code;
  std::vector<Token> tokens;
  size_t currentTokenIndex = 0;
  Token currentToken;

  std::vector<Errors::Error> errors;

  std::unordered_map<std::string, TypePtr> symbolTable;

  AstNodePtr ParseStructDefinition();
  AstNodePtr ParseStructMemberDefinition();
  AstNodePtr ParseStructInit();
  AstNodePtr ParseStructMemberInit();
  AstNodePtr ParseFunctionDeclaration();
  std::vector<AstNodePtr> ParseFunctionParameters();
  AstNodePtr ParseFunctionBody();
  AstNodePtr ParseStatement();
  AstNodePtr ParseIfElseStatement();
  AstNodePtr ParseBlock();
  AstNodePtr ParseReturn();
  AstNodePtr ParseWhileLoop();
  AstNodePtr ParseDoWhileLoop();
  AstNodePtr ParseForLoop();
  AstNodePtr ParseExpression();
  AstNodePtr ParseAssignment();
  AstNodePtr ParseReassignment();
  AstNodePtr ParseEqualityExpression();
  AstNodePtr ParseAddExpression();
  AstNodePtr ParseMulExpression();
  AstNodePtr ParseUnaryExpression();
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
  template<class... T>
  std::unique_ptr<Token> Expect(TokenType tokenType, Errors::ErrorType errorType, T&& ... args);
  template<class... T>
  std::unique_ptr<Token> ExpectOneOf(const std::list<TokenType>& possibleTokenTypes, Errors::ErrorType errorType, T&& ... args);
  std::unique_ptr<Token> ExpectDataType();


  /**
   * Checks if the specified token is of the specified type.
   * @param lookaheadDistance How far ahead in the list to look for the token
   * @param tokenType The token to match
   * @return Whether or not the token was found
   */
  bool Peek(size_t lookaheadDistance, TokenType tokenType);
  bool PeekOneOf(size_t lookaheadDistance, const std::list<TokenType>& possibleTokenTypes);

  /**
   * Checks if the specified token is of the specified type, reports an error otherwise.
   * @param lookaheadDistance How far ahead in the list to look for the token
   * @param tokenType The token to match
   * @param errorMessage The error message to display if the token is not found
   * @return
   */
  template<class... T>
  bool PeekWithError(size_t lookaheadDistance, TokenType tokenType, Errors::ErrorType errorType, T&& ... args);

  /**
   * Panic-mode recovery
   * When an error is found, skip tokens until a recovery token is hit and consumes that token
   * Ideally, update this to be more context based and/or add phrase-level recovery
   */
  void Recover(TokenType synchronizationToken);
  /**
   * Panic-mode recovery
   * When an error is found, skip tokens until a recovery token is hit without consuming that token.
   * This is useful for example with function parameters where we want to recover at the ')' token but
   * the consuming of that token is handled within the calling function.
   */
  void RecoverKeepSynchronizationToken(TokenType synchronizationToken);
  void RecoverWithOneOf(const std::set<TokenType>& synchronizationTokens);

  /**
   * Advance the currentTokenIndex and update currentToken
   */
  void Advance();

  /**
   * Returns true if currentToken is TK_EOF
   */
  bool IsFinishedParsing();
  void IgnoreNewlines();
  size_t GetNewLineCountFromCurrentToken();

  template<class... T>
  void LogError(Errors::ErrorType errorType, T&& ... args);

  template<class... T>
  void LogErrorAtPosition(Errors::ErrorType errorType, Location location, T&& ... args);
};

#endif //KODAMA_SRC_PARSER_PARSER_HPP_