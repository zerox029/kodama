//
// Created by emma on 02/10/23.
//

#ifndef KODAMA_KODAMA_CLI_CLI_HPP_
#define KODAMA_KODAMA_CLI_CLI_HPP_

#include <vector>
#include <string>

namespace cli {
struct CliState {
  std::string inputFileName{};
  std::string outputFileName{};

  bool emitLLVM = false;

  void ValidateState();
};

class CLI {
 public:
  CLI(int argc, char** argv);
  CliState ParseArguments();
 private:
  std::vector<std::string> args;
  CliState state;

  void Help();
};
}

#endif //KODAMA_KODAMA_CLI_CLI_HPP_
