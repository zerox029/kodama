#include "compiler/compiler.hpp"
#include "cli/cli.hpp"

int
main(int argc, char** argv) {
  cli::CLI cli(argc, argv);
  cli::CliState cliState = cli.ParseArguments();
  cliState.ValidateState();

  Compiler compiler{};
  compiler.Compile(cliState);



  return 0;
}
