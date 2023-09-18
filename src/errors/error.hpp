//
// Created by emma on 18/09/23.
//

#ifndef KODAMA_SRC_ERRORS_ERROR_HPP_
#define KODAMA_SRC_ERRORS_ERROR_HPP_

#include <string>

enum ErrorType {
  TOKENIZATION_ERROR
};

struct ErrorLocation {
  size_t lineNumber;
  size_t characterNumber;
};

class Error {
 private:
  ErrorType errorType;
  ErrorLocation errorLocation;

 public:
  Error(ErrorType errorType, ErrorLocation errorLocation);
  void Throw(ErrorType error_type);
};

#endif //KODAMA_SRC_ERRORS_ERROR_HPP_
