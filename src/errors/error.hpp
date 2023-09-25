//
// Created by emma on 18/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERROR_HPP_
#define KODAMA_SRC_ERRORS_ERROR_HPP_

#include "../lexer/lexer.hpp"
#include <string>

namespace errorStrings {
inline constexpr auto EXPECTED_IDENTIFIER = "expected identifier but got '{0}'";
inline constexpr auto EXPECTED_OP_DELIMITER = "expected opening delimiter '{0}' but got '{1}'";
inline constexpr auto EXPECTED_CL_DELIMITER = "expected closing delimiter '{0}' but got '{1}'";
inline constexpr auto EXPECTED_KEYWORD = "expected keyword '{0}' but got '{1}'";
inline constexpr auto EXPECTED_TOKEN = "expected token '{0}' but got '{1}'";
inline constexpr auto EXPECTED_DATATYPE = "expected datatype";
}

class Error {
 private:
  std::string errorType;
  std::string errorMessage;
  Location errorLocation;
  std::string codeLine;

 public:
  Error(std::string errorType, std::string errorMessage, Location errorLocation, std::string codeLine);
  void Throw() const;

  const std::string& GetErrorType() const;
  const std::string& GetErrorMessage() const;
  const Location& GetErrorLocation() const;
  const std::string& GetCodeLine() const;
};

#endif //KODAMA_SRC_ERRORS_ERROR_HPP_
