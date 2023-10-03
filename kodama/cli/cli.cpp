//
// Created by emma on 02/10/23.
//

#include "cli.hpp"
#include <iostream>

namespace cli {
void
CliState::ValidateState() {
  if (inputFileName == "") {
    std::cout << "No input file specified.\nUse 'kodama --help' for more information" << std::endl;
    exit(1);
  }
  if(emitLLVM && outputFileName.empty()) {
    outputFileName = inputFileName.substr(0, inputFileName.size() - 4)+".ll";
  }
  else if(!emitLLVM && outputFileName.empty()) {
    outputFileName = inputFileName.substr(0, inputFileName.size() - 4)+".o";
  }
}

CLI::CLI(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }
}

void
CLI::Help() {
  std::cout << "Usage: kodama [command] [options] [file] -o [output file]\n";
}

CliState
CLI::ParseArguments() {
  for (int i = 0; i < args.size(); i++) {
    const std::string& arg = args.at(i);

    if (arg == "--emit-llvm") {
      state.emitLLVM = true;
    }
    else if(arg == "--skip-optimizations") {
      state.skipOptimizations = true;
    }
    else if (arg == "--help" || arg == "-h") {
      Help();
      exit(0);
    }
    else if(arg == "-o") {
      if (!state.outputFileName.empty()) {
        std::cout << "Error: Only one output file can be specified" << std::endl;
        exit(1);
      }

      if(i < args.size() - 1) {
        state.outputFileName = args[++i];
      }
      else {
        std::cout << "Error: No output file specified" << std::endl;
        exit(1);
      }
    }
    else {
      if (arg.substr(arg.size() - 4, 4) == ".kdm") {
        if (!state.inputFileName.empty()) {
          std::cout << "Error: Only one input file can be specified" << std::endl;
          exit(1);
        }

        state.inputFileName = arg;
      }
    }
  }

  return state;
}
}