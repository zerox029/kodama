//
// Created by emma on 24/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_
#define KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_

#include "error.hpp"
#include <memory>
#include <utility>
#include <fmt/format.h>
#include <format>

namespace Errors {
enum ErrorType {
  // Syntax
  EXPECTED_IDENTIFIER,
  EXPECTED_OP_DELIMITER,
  EXPECTED_CL_DELIMITER,
  EXPECTED_KEYWORD,
  EXPECTED_TOKEN,
  EXPECTED_DATATYPE,
  EXPECTED_VALUE_IDENTIFIER,
  UNTERMINATED_STRING,

  // Semantic
  ID_NOT_FOUND,
  ASSIGN_VAL,
  ILLEGAL_REDEFINITION,

  // Type
};

template<class... T>
std::pair<std::string, std::string>
ErrorMessage(const ErrorType errorType, T&& ... args) {
  switch (errorType) {
    // Syntax errors
    case EXPECTED_IDENTIFIER:
      return std::make_pair("syntax error", fmt::vformat("expected identifier but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_OP_DELIMITER:
      return std::make_pair("syntax error", fmt::vformat("expected opening delimiter '{}' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_CL_DELIMITER:
      return std::make_pair("syntax error", fmt::vformat("expected closing delimiter '{}' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_KEYWORD:
      return std::make_pair("syntax error", fmt::vformat("expected keyword '{}' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_TOKEN:
      return std::make_pair("syntax error", fmt::vformat("expected token '{}' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_DATATYPE:
      return std::make_pair("syntax error", "expected datatype");
    case EXPECTED_VALUE_IDENTIFIER:
      return std::make_pair("syntax error", "expected value or identifier");
    case UNTERMINATED_STRING:
      return std::make_pair("syntax error", "unterminated string");

    // Semantic
    case ID_NOT_FOUND:
      return std::make_pair("error", fmt::vformat("cannot find symbol '{}' in scope",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case ASSIGN_VAL:
      return std::make_pair("error", fmt::vformat("cannot assign twice to value '{}'",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case ILLEGAL_REDEFINITION:
      return std::make_pair("error", fmt::vformat("illegal redefinition of '{}'",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
  }
}

template<class... T>
Error
Generate(ErrorType errorType, const Location& location, const std::vector<std::string>& code, const T&& ... args) {
  std::pair<std::string, std::string> message = ErrorMessage(errorType, args...);

  return Error{message.first, message.second, location, code.at(location.lineNumber)};
}

Error
Expected(const std::string& errorMessage, const Token& actual, const std::vector<std::string>& code);
Error
Expected(TokenType expected, const Token& actual, const std::vector<std::string>& code);
Error
Expected(TokenType expected, const std::vector<std::string>& code);
}

#endif //KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_
