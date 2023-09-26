//
// Created by emma on 24/09/23.
//

#include "errorFactory.hpp"
#include "../lexer/lexer.hpp"
#include <string>
#include <format>

namespace Errors {
Error
Expected(const std::string& errorMessage, const Token& actual, const std::vector<std::string>& code) {
  Location location = actual.GetLocation();
  location.lineNumber += 1;
  location.characterLineIndex += 1;

  return Error{"syntax error", errorMessage, location, code.at(actual.GetLocation().lineNumber)};
}

Error
Expected(const TokenType expected, const Token& actual, const std::vector<std::string>& code) {
  std::string message{};
  switch (expected) {
    case TK_EOF:
      return Expected(expected, code);
    case TK_IDENTIFIER:
      message = std::format(errorStrings::EXPECTED_IDENTIFIER, actual.GetStr());
      break;
    case TK_OPEN_PAREN:
    case TK_OPEN_CURLY:
      message = std::format(errorStrings::EXPECTED_OP_DELIMITER, Lexer::GetSymbolFromTokenType(expected), actual.GetStr());
      break;
    case TK_CLOSED_PAREN:
    case TK_CLOSED_CURLY:
      message = std::format(errorStrings::EXPECTED_CL_DELIMITER, Lexer::GetSymbolFromTokenType(expected), actual.GetStr());
      break;
    case TK_WHILE:
      message = std::format(errorStrings::EXPECTED_KEYWORD, Lexer::GetSymbolFromTokenType(expected), actual.GetStr());
      break;
    default:
      message = std::format(errorStrings::EXPECTED_TOKEN, Lexer::GetSymbolFromTokenType(expected), actual.GetStr());
      break;
  }

  return Expected(message, actual, code);
}

Error
Expected(const TokenType tokenType, const std::vector<std::string>& code) {

}
}