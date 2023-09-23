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


Error::Error(std::string errorType, std::string errorMessage, Location errorLocation, std::string codeLine)
    : errorType{std::move(errorType)},
      errorMessage{std::move(errorMessage)},
      errorLocation{std::move(errorLocation)},
      codeLine{std::move(codeLine)} {}

void
Error::Throw() const {
  std::cout << "\033[90m"
            << errorLocation.filePath
            << "(" << errorLocation.lineNumber + 1 << ":" << errorLocation.characterLineIndex + 1 << "): "
            << errorType + ": " + errorMessage
            << "\033[0m\n";

  std::cout << codeLine << std::endl;
  std::cout << AddSpace("^", errorLocation.characterLineIndex + 1) << "^";

  exit(1);
}