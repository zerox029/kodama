//
// Created by emma on 25/09/23.
//

#include "../../parser/parser.hpp"
#include "../../compiler/compiler.hpp"
#include <gtest/gtest.h>
#include <string>

constexpr std::string DUMMY_FILE_LOCATION = "dummyLocation";

int
getProgramReturnCode(const std::string& code) {
  Compiler compiler{};
  compiler.Compile(code);

  return system("lli ../out/out.ll");
}

TEST(addition, PositiveIntegerAndPositiveInteger) {
  // GIVEN
  std::string code = "def main() -> i64 { return 1 + 1 }";

  // WHEN
  int returnCode = getProgramReturnCode(code);

  // THEN
  EXPECT_EQ(WEXITSTATUS(returnCode), 2);
}

TEST(substraction, PositiveIntegerAndPositiveIntegerPositiveResult) {
  // GIVEN
  std::string code = "def main() -> i64 { return 5 - 2 }";

  // WHEN
  int returnCode = getProgramReturnCode(code);

  // THEN
  EXPECT_EQ(WEXITSTATUS(returnCode), 3);
}

// Find a way to do this
TEST(substraction, PositiveIntegerAndPositiveIntegerNegativeResult) {
  // GIVEN
  std::string code = "def main() -> i64 { return 2 - 5 }";

  // WHEN
  int returnCode = getProgramReturnCode(code);

  // THEN
  EXPECT_EQ(WEXITSTATUS(returnCode), -3);
}