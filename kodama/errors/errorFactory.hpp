//
// Created by emma on 24/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_
#define KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_

#include "error.hpp"
#include <memory>

enum errorClass {
  SYNTAX,
  SEMANTIC,
  TYPE,
};

class ErrorFactory {
 public:
  static Error Expected(const std::string& errorMessage, const Token& actual, const std::vector<std::string>& code);
  static Error Expected(TokenType expected, const Token& actual, const std::vector<std::string>& code);
  static Error Expected(TokenType expected, const std::vector<std::string>& code);
};


#endif //KODAMA_SRC_ERRORS_ERRORFACTORY_HPP_
