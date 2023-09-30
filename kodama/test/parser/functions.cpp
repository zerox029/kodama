//
// Created by emma on 24/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(FunctionDeclaration, HappyPath) {
  // GIVEN
  std::string code = "def test() -> i32 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());

  // THEN
  EXPECT_EQ(program->GetStatements().size(), 1);
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FUNC_DEC);
}

TEST(FunctionDeclaration, MissingIdentifier) {
  // GIVEN
  std::string code = "def () -> i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_IDENTIFIER, "(");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 5};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}


TEST(FunctionDeclaration, MissingOpeningParen) {
  // GIVEN
  std::string code = "def test) -> i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_OP_DELIMITER, "(", ")");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 9};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingClosingParen) {
  // GIVEN
  std::string code = "def test( -> i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_CL_DELIMITER, ")", "->");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 11};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingReturnArrow) {
  // GIVEN
  std::string code = "def test() i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_TOKEN, "->", "i32");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 12};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
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
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(), Errors::Strings::EXPECTED_DATATYPE);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, MissingOpeningCurly) {
  // GIVEN
  std::string code = "def test() -> i32 }";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_FUNCTION_BODY, "}");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 19};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(), expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionDeclaration, AbortAfterFirstError) {
  // GIVEN
  std::string code = "def test) -> i32 }";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_OP_DELIMITER, "(", ")");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 9};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionParameters, HappyPathOneParameter) {
  // GIVEN
  std::string code = "def test(param: i32) -> i32 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* functionDeclaration = dynamic_cast<FunctionDeclaration*>(program->GetStatements().at(0).get());
  std::vector<AstNodePtr> parameters = functionDeclaration->GetParameters();

  auto* parameter = dynamic_cast<FunctionParameter*>(parameters.at(0).get());

  // THEN
  EXPECT_EQ(parameters.size(), 1);
  EXPECT_EQ(parameter->GetIdentifier(), "param");
  EXPECT_EQ(parameter->GetDataType()->GetTypeName(), I32_TYPE);
}

TEST(FunctionParameters, HappyPathTwoParameter) {
  // GIVEN
  std::string code = "def test(param: f64, paramTwo: bool) -> i32 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* functionDeclaration = dynamic_cast<FunctionDeclaration*>(program->GetStatements().at(0).get());
  std::vector<AstNodePtr> parameters = functionDeclaration->GetParameters();

  auto* parameterOne = dynamic_cast<FunctionParameter*>(parameters.at(0).get());
  auto* parameterTwo = dynamic_cast<FunctionParameter*>(parameters.at(1).get());

  // THEN
  EXPECT_EQ(parameters.size(), 2);

  EXPECT_EQ(parameterOne->GetIdentifier(), "param");
  EXPECT_EQ(parameterOne->GetDataType()->GetTypeName(), F64_TYPE);

  EXPECT_EQ(parameterTwo->GetIdentifier(), "paramTwo");
  EXPECT_EQ(parameterTwo->GetDataType()->GetTypeName(), BOOL_TYPE);
}

TEST(FunctionParameters, MissingColon) {
  // GIVEN
  std::string code = "def test(param) -> i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_TOKEN, ":", ")");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 15};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(FunctionParameters, MissingType) {
  // GIVEN
  std::string code = "def test(param:) -> i32 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_DATATYPE);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 16};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}