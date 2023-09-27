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
  EXPECTED_FUNCTION_BODY,
  EXPECTED_TO_UNTIL,

  // Semantic
  ID_NOT_FOUND,
  ASSIGN_VAL,
  ILLEGAL_REDEFINITION,
  UNASSIGNED_VALUE,
  INVALID_OUTSIDE_FUNCTION,
  INVALID_INSIDE_FUNCTION,
  UNREACHABLE_CODE,
  INVALID_POSITIONAL_ARGUMENT,
  LIST_BOUNDS_INVERTED,

  // Type
  RETURN_TYPE_MISMATCH,
  VAR_TYPE_MISMATCH
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
    case EXPECTED_FUNCTION_BODY:
      return std::make_pair("syntax error", fmt::vformat("expected function body delimiter '=>' or '{' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_TO_UNTIL:
      return std::make_pair("syntax error", fmt::vformat("expected keyword 'to' or 'until' but got '{}'",
                                                         fmt::make_format_args(std::forward<T>(args)...)));

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
    case UNASSIGNED_VALUE:
      return std::make_pair("error", "expression result unused. Assign it or remove it.");
    case INVALID_OUTSIDE_FUNCTION:
      return std::make_pair("error", "out of place expression. use it inside a function");
    case INVALID_INSIDE_FUNCTION:
      return std::make_pair("error", fmt::vformat("{} is not allowed inside a function",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case UNREACHABLE_CODE:
      return std::make_pair("error", fmt::vformat("everything after this statement is unreachable",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case INVALID_POSITIONAL_ARGUMENT:
      return std::make_pair("error", fmt::vformat("positional argument after named argument",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case LIST_BOUNDS_INVERTED:
      return std::make_pair("error", fmt::vformat("list lower bound '{}' is greater than the greater bound '{}'",
                                                  fmt::make_format_args(std::forward<T>(args)...)));

    // Type
    case RETURN_TYPE_MISMATCH:
      return std::make_pair("type mismatch", fmt::vformat("expected return type '{}' but got '{}'",
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case VAR_TYPE_MISMATCH:
      return std::make_pair("type mismatch", fmt::vformat("expected type '{}' but got '{}'",
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
