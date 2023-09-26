#include "compiler/compiler.hpp"
#include "utils/stringUtils.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <variant>
#include <fmt/format.h>
#include <utility>

std::string
ReadFile() {
  std::ifstream inputFile("../kodama/code.kdm");
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

int
main() {
  Compiler compiler{};
  compiler.Compile(ReadFile());

  system("lli ../out/out.ll");

  return 0;
}
