//
// Created by emma on 18/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERROR_HPP_
#define KODAMA_SRC_ERRORS_ERROR_HPP_

#include "../lexer/lexer.hpp"
#include <string>

enum ErrorType {
  TOKENIZATION_ERROR
};

class Error {
 private:
  std::string errorType;
  std::string errorMessage;
  Location errorLocation;
  std::string codeLine;

 public:
  Error(std::string errorType, std::string errorMessage, Location errorLocation, std::string codeLine);
  void Throw() const;
};

#endif //KODAMA_SRC_ERRORS_ERROR_HPP_
