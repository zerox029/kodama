//
// Created by emma on 29/09/23.
//

#include "testUtils.hpp"

AstNodePtr
buildAST(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, DUMMY_FILE_LOCATION};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{codeLines, tokens};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();

  return get<AstNodePtr>(parseResult);
}

std::vector<Errors::Error>
getErrorsForAst(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, DUMMY_FILE_LOCATION};
  std::vector<Token> tokens = lexer.Tokenize();

  Parser parser{codeLines, tokens};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();

  return get<std::vector<Errors::Error>>(parseResult);
}

void AssertEqLocation(const Location& expected, const Location& actual) {
  EXPECT_EQ(expected.lineNumber, actual.lineNumber);
  EXPECT_EQ(expected.characterLineIndex, actual.characterLineIndex);
  EXPECT_EQ(expected.filePath, actual.filePath);
}