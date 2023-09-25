//
// Created by emma on 24/09/23.
//

#include "../../src/parser/parser.hpp"
#include "../../src/utils/stringUtils.hpp"
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

TEST(FunctionDeclaration, MissingIdentifier) {
  // GIVEN
  std::string code = "def () -> i32 {}";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_IDENTIFIER, "(");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 5};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}


TEST(FunctionDeclaration, MissingOpeningParen) {
  // GIVEN
  std::string code = "def test) -> i32 {}";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_OP_DELIMITER, "(", ")");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 9};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingClosingParen) {
  // GIVEN
  std::string code = "def test( -> i32 {}";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_CL_DELIMITER, ")", "->");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 11};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingReturnArrow) {
  // GIVEN
  std::string code = "def test() i32 {}";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_TOKEN, "->", "i32");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 12};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingReturnType) {
  // GIVEN
  std::string code = "def test() -> {}";
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 15};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(), errorStrings::EXPECTED_DATATYPE);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingOpeningCurly) {
  // GIVEN
  std::string code = "def test() -> i32 }";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_OP_DELIMITER, "{", "}");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 19};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(), expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, AbortAfterFirstError) {
  // GIVEN
  std::string code = "def test) -> i32 }";
  std::string expectedMessage = std::format(errorStrings::EXPECTED_OP_DELIMITER, "(", ")");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 9};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorType(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}