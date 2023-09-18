//
// Created by emma on 18/09/23.
//

#include "error.hpp"
#include <iostream>

Error::Error(ErrorType errorType, ErrorLocation errorLocation) : errorType{errorType}, errorLocation{errorLocation} {}

void
Error::Throw(ErrorType error_type) {
  switch (error_type) {
    case TOKENIZATION_ERROR:
      std::cout << "Encountered an error tokenizing at line " << errorLocation.lineNumber << std::endl;
  }
  
  exit(1);
}