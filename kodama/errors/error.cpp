//
// Created by emma on 18/09/23.
//

/**
 * -- Error format --
 * {filePath}({lineNumber}:{characterIndex}): {errorType}: {errorMessage}
 * {codeLine}{problematicCharacter}
 *                  ^
 *
 * -- Example error --
 * /a/file/location/code.kdm:4:20: syntax error: expected ')'
 * printf("hello world"
 *                     ^
 */

#include "error.hpp"
#include "../utils/stringUtils.hpp"
#include <iostream>
#include <utility>

namespace Errors {
Error::Error(std::string errorType, std::string errorMessage, Location errorLocation, std::string codeLine)
    : errorClass{std::move(errorType)},
      errorMessage{std::move(errorMessage)},
      errorLocation{std::move(errorLocation)},
      codeLine{std::move(codeLine)} {}

void
Error::Throw() const {
  std::string trimmedCode = LeftTrim(codeLine);
  size_t removedSpaces = codeLine.length() - trimmedCode.length();

  std::cout << "\033[90m"
            << errorLocation.filePath
            << "(" << errorLocation.lineNumber << ":" << errorLocation.characterLineIndex << "): "
            << errorClass + ": " + errorMessage
            << "\033[0m\n";

  std::cout << trimmedCode << std::endl;
  std::cout << AddSpace("", errorLocation.characterLineIndex - removedSpaces) << "^" << "\n\n";
}

const std::string&
Error::GetErrorClass() const {
  return errorClass;
}

const std::string&
Error::GetErrorMessage() const {
  return errorMessage;
}

const Location&
Error::GetErrorLocation() const {
  return errorLocation;
}

const std::string&
Error::GetCodeLine() const {
  return codeLine;
}
}
