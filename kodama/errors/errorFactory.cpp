//
// Created by emma on 30/09/23.
//

#include "errorFactory.hpp"

namespace Errors {
Error
Generate(ErrorType errorType, Location location, const std::vector<std::string>& code) {
  std::pair<std::string, std::string> message = ErrorMessage(errorType);

  location.lineNumber += 1;
  location.characterLineIndex += 1;

  return Error{message.first, message.second, location, code.at(location.lineNumber - 1)};
}
}