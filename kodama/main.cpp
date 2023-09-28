#include "compiler/compiler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
  std::string code = ReadFile();

  Compiler compiler{};
  compiler.Compile(code);

  system("lli ../out/out.ll");

  return 0;
}
