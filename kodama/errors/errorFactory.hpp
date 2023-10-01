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
#include <vector>

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
  UNEXPECTED_EXPRESSION,
  UNEXPECTED_TOKEN,
  EXPECTED_CONDITION,

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
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_IDENTIFIER,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_OP_DELIMITER:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_OP_DELIMITER,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_CL_DELIMITER:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_CL_DELIMITER,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_KEYWORD:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_KEYWORD,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_TOKEN:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_TOKEN,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_DATATYPE:
      return std::make_pair("syntax error", Strings::EXPECTED_DATATYPE);
    case EXPECTED_VALUE_IDENTIFIER:
      return std::make_pair("syntax error", Strings::EXPECTED_VALUE_IDENTIFIER);
    case UNTERMINATED_STRING:
      return std::make_pair("syntax error", Strings::UNTERMINATED_STRING);
    case EXPECTED_FUNCTION_BODY:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_FUNCTION_BODY,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_TO_UNTIL:
      return std::make_pair("syntax error", fmt::vformat(Strings::EXPECTED_TO_UNTIL,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case UNEXPECTED_EXPRESSION:
      return std::make_pair("syntax error", fmt::vformat(Strings::UNEXPECTED_EXPRESSION,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case UNEXPECTED_TOKEN:
      return std::make_pair("syntax error", fmt::vformat(Strings::UNEXPECTED_TOKEN,
                                                         fmt::make_format_args(std::forward<T>(args)...)));
    case EXPECTED_CONDITION:
      return std::make_pair("syntax error", Strings::EXPECTED_CONDITION);

      // Semantic
    case ID_NOT_FOUND:
      return std::make_pair("error", fmt::vformat(Strings::ID_NOT_FOUND,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case ASSIGN_VAL:
      return std::make_pair("error", fmt::vformat(Strings::ASSIGN_VAL,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case ILLEGAL_REDEFINITION:
      return std::make_pair("error", fmt::vformat(Strings::ILLEGAL_REDEFINITION,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case UNASSIGNED_VALUE:
      return std::make_pair("error", Strings::UNASSIGNED_VALUE);
    case INVALID_OUTSIDE_FUNCTION:
      return std::make_pair("error", Strings::INVALID_OUTSIDE_FUNCTION);
    case INVALID_INSIDE_FUNCTION:
      return std::make_pair("error", fmt::vformat(Strings::INVALID_INSIDE_FUNCTION,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case UNREACHABLE_CODE:
      return std::make_pair("error", fmt::vformat(Strings::UNREACHABLE_CODE,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case INVALID_POSITIONAL_ARGUMENT:
      return std::make_pair("error", fmt::vformat(Strings::INVALID_POSITIONAL_ARGUMENT,
                                                  fmt::make_format_args(std::forward<T>(args)...)));
    case LIST_BOUNDS_INVERTED:
      return std::make_pair("error", fmt::vformat(Strings::LIST_BOUNDS_INVERTED,
                                                  fmt::make_format_args(std::forward<T>(args)...)));

      // Type
    case RETURN_TYPE_MISMATCH:
      return std::make_pair("type mismatch", fmt::vformat(Strings::RETURN_TYPE_MISMATCH,
                                                          fmt::make_format_args(std::forward<T>(args)...)));
    case VAR_TYPE_MISMATCH:
      return std::make_pair("type mismatch", fmt::vformat(Strings::VAR_TYPE_MISMATCH,
                                                          fmt::make_format_args(std::forward<T>(args)...)));
  }
}

template<class... T>
Error
Generate(ErrorType errorType, Location location, const std::vector<std::string>& code, const T&& ... args) {
  std::pair<std::string, std::string> message = ErrorMessage(errorType, args...);

  location.lineNumber += 1;
  location.characterLineIndex += 1;

  return Error{message.first, message.second, location, code.at(location.lineNumber - 1)};
}

Error
Generate(ErrorType errorType, Location location, const std::vector<std::string>& code);
}

#endif //KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_