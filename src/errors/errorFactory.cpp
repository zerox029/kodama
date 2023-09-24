//
// Created by emma on 24/09/23.
//

#include "errorFactory.hpp"
#include "../lexer/lexer.hpp"
#include <string>

Error
ErrorFactory::Expected(const std::string& errorMessage, const Token& actual, const std::vector<std::string>& code) {
  return Error{"syntax error", errorMessage, actual.GetLocation(), code.at(actual.GetLocation().lineNumber)};
}

Error
ErrorFactory::Expected(const TokenType expected, const Token& actual, const std::vector<std::string>& code) {
  std::string message{};
  switch (expected) {
    case TK_EOF:
      return Expected(expected, code);
    case TK_IDENTIFIER:
       message = "expected identifier but got '" + actual.GetStr() + "'";
       break;
    case TK_OPEN_PAREN:
    case TK_OPEN_CURLY:
      message = "expected opening delimiter '" + Lexer::GetSymbolFromTokenType(expected) + "' but got '" + actual.GetStr() + "'";
      break;
    case TK_CLOSED_PAREN:
    case TK_CLOSED_CURLY:
      message = "expected closing delimiter '" + Lexer::GetSymbolFromTokenType(expected) + "' but got '" + actual.GetStr() + "'";
      break;
    case TK_WHILE:
      message = "expected keyword '" + Lexer::GetSymbolFromTokenType(expected) + "' but got '" + actual.GetStr() + "'";
      break;
    default:
      message = "expected token '" + Lexer::GetSymbolFromTokenType(expected) + "' but got '" + actual.GetStr() + "'";
      break;
  }

  return Error{"syntax error", message, actual.GetLocation(), code.at(actual.GetLocation().lineNumber)};
}

Error
ErrorFactory::Expected(const TokenType tokenType, const std::vector<std::string>& code) {

}