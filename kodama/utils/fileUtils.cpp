//
// Created by emma on 02/10/23.
//

#include "fileUtils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string
ReadFile(const std::string& location) {
  std::ifstream inputFile(location);
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file.\n";
    return "";
  }

  std::string line;
  std::stringstream codeStream;
  while (std::getline(inputFile, line)) {
    codeStream << line << "\n";
  }

  inputFile.close();

  return codeStream.str();
}