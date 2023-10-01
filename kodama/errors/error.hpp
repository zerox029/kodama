//
// Created by emma on 18/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERROR_HPP_
#define KODAMA_SRC_ERRORS_ERROR_HPP_

#include "../lexer/token.hpp"
#include <string>

namespace Errors {
namespace Strings {
// Syntax
inline constexpr auto EXPECTED_IDENTIFIER = "expected identifier but got '{}'";
inline constexpr auto EXPECTED_OP_DELIMITER = "expected opening delimiter '{}' but got '{}'";
inline constexpr auto EXPECTED_CL_DELIMITER = "expected closing delimiter '{}' but got '{}'";
inline constexpr auto EXPECTED_KEYWORD = "expected keyword '{}' but got '{}'";
inline constexpr auto EXPECTED_TOKEN = "expected token '{}' but got '{}'";
inline constexpr auto EXPECTED_DATATYPE = "expected datatype";
inline constexpr auto EXPECTED_VALUE_IDENTIFIER = "expected value or identifier";
inline constexpr auto UNTERMINATED_STRING = "unterminated string";
inline constexpr auto EXPECTED_FUNCTION_BODY = "expected function body delimiter '=>' or '{{' but got '{}'";
inline constexpr auto EXPECTED_TO_UNTIL = "expected keyword 'to' or 'until' but got '{}'";
inline constexpr auto UNEXPECTED_EXPRESSION = "unexpected expression '{}'";
inline constexpr auto UNEXPECTED_TOKEN = "unexpected token '{}'";
inline constexpr auto EXPECTED_CONDITION = "expected condition";
inline constexpr auto UNMATCHED_OP_DELIMITER = "unmatched opening delimiter '{}'";

// Semantic
inline constexpr auto ID_NOT_FOUND = "cannot find symbol '{}' in scope";
inline constexpr auto ASSIGN_VAL = "cannot assign twice to value '{}'";
inline constexpr auto ILLEGAL_REDEFINITION = "illegal redefinition of '{}'";
inline constexpr auto UNASSIGNED_VALUE = "expression result unused. Assign it or remove it";
inline constexpr auto INVALID_OUTSIDE_FUNCTION = "out of place expression. use it inside a function";
inline constexpr auto INVALID_INSIDE_FUNCTION = "{} is not allowed inside a function";
inline constexpr auto UNREACHABLE_CODE = "everything after this statement is unreachable";
inline constexpr auto INVALID_POSITIONAL_ARGUMENT = "positional argument after named argument";
inline constexpr auto LIST_BOUNDS_INVERTED = "list lower bound '{}' is greater than the greater bound '{}'";

// Type
inline constexpr auto RETURN_TYPE_MISMATCH = "expected return type '{}' but got '{}'";
inline constexpr auto VAR_TYPE_MISMATCH = "expected type '{}' but got '{}'";
}

class Error {
 private:
  std::string errorClass;
  std::string errorMessage;
  Location errorLocation;
  std::string codeLine;

 public:
  Error(std::string errorType, std::string errorMessage, Location errorLocation, std::string codeLine);
  void Throw() const;

  const std::string& GetErrorClass() const;
  const std::string& GetErrorMessage() const;
  const Location& GetErrorLocation() const;
  const std::string& GetCodeLine() const;
};
}

#endif //KODAMA_SRC_ERRORS_ERROR_HPP_
