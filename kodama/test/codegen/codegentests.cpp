//
// Created by emma on 25/09/23.
//


//
// Created by emma on 24/09/23.
//

#include "../../parser/parser.hpp"
#include "../../utils/stringUtils.hpp"
#include <gtest/gtest.h>
#include <string>
#include <format>

constexpr std::string DUMMY_FILE_LOCATION = "dummyLocation";

AstNodePtr
buildAST(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, DUMMY_FILE_LOCATION};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{codeLines, tokens};
  std::variant<AstNodePtr, std::vector<Error>> parseResult = parser.Parse();

  return get<AstNodePtr>(parseResult);
}

std::vector<Error>
getErrorsForAst(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, DUMMY_FILE_LOCATION};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{codeLines, tokens};
  std::variant<AstNodePtr, std::vector<Error>> parseResult = parser.Parse();

  return get<std::vector<Error>>(parseResult);
}

void AssertEqLocation(const Location& expected, const Location& actual) {
  EXPECT_EQ(expected.lineNumber, actual.lineNumber);
  EXPECT_EQ(expected.characterLineIndex, actual.characterLineIndex);
  EXPECT_EQ(expected.filePath, actual.filePath);
}

TEST(FunctionDeclaration, HappyPath) {
  // GIVEN
  std::string code = "def test() -> i32 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  Block* program = dynamic_cast<Block*>(ast.get());

  // THEN
  EXPECT_EQ(program->GetStatements().size(), 1);
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FUNC_DEC);
}