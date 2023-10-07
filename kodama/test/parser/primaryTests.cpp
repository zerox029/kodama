//
// Created by emma on 30/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(String, HappyPath) {
  // GIVEN
  std::string code = "\"test\";";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* stringNode = dynamic_cast<StringLiteral*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_STRING_LITERAL);
  EXPECT_EQ(stringNode->GetValue(), "test");
}

TEST(Boolean, TrueHappyPath) {
  // GIVEN
  std::string code = "true;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* boolNode = dynamic_cast<BoolValue*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BOOL_VALUE);
  EXPECT_TRUE(boolNode->GetValue());
}

TEST(Boolean, FalseHappyPath) {
  // GIVEN
  std::string code = "false;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* boolNode = dynamic_cast<BoolValue*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BOOL_VALUE);
  EXPECT_FALSE(boolNode->GetValue());
}

TEST(Number, IntegerHappyPath) {
  // GIVEN
  std::string code = "55;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* integerNode = dynamic_cast<IntegerLiteral*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(integerNode->GetValue(), "55");
}

TEST(Number, DecimalHappyPath) {
  // GIVEN
  std::string code = "55.64;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* decimalLiteral = dynamic_cast<DecimalLiteral*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_DECIMAL_LITERAL);
  EXPECT_EQ(decimalLiteral->GetValue(), "55.64");
  EXPECT_EQ(decimalLiteral->GetIntegerValue(), "55");
  EXPECT_EQ(decimalLiteral->GetDecimalValue(), "64");
}

TEST(Identifier, HappyPath) {
  // GIVEN
  std::string code = "identifierTest;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* identifier = dynamic_cast<Variable*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_VARIABLE);
  EXPECT_EQ(identifier->GetIdentifier(), "identifierTest");
}

TEST(Null, HappyPath) {
  // GIVEN
  std::string code = "null;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_NULL_VALUE);
}

TEST(Call, HappyPathNoArgument) {
  // GIVEN
  std::string code = "test();";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* call = dynamic_cast<FunctionCall*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FUNCTION_CALL);
  EXPECT_EQ(call->GetArguments().size(), 0);
  EXPECT_EQ(call->GetIdentifier(), "test");
}

TEST(Call, HappyPathWithUnnamedArguments) {
  // GIVEN
  std::string code = "test(14, \"test\", true);";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* call = dynamic_cast<FunctionCall*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FUNCTION_CALL);
  EXPECT_EQ(call->GetArguments().size(), 3);
  EXPECT_EQ(call->GetArguments().at(0)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(0).get())->GetIdentifier(), "");
  EXPECT_EQ(call->GetArguments().at(1)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(1).get())->GetIdentifier(), "");
  EXPECT_EQ(call->GetArguments().at(2)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(2).get())->GetIdentifier(), "");
  EXPECT_EQ(call->GetIdentifier(), "test");
}

TEST(Call, HappyPathWithNamedArguments) {
  // GIVEN
  std::string code = "test(arg: 14, otherArg: \"test\", ok: true);";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* call = dynamic_cast<FunctionCall*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FUNCTION_CALL);
  EXPECT_EQ(call->GetArguments().size(), 3);
  EXPECT_EQ(call->GetArguments().at(0)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(0).get())->GetIdentifier(), "arg");
  EXPECT_EQ(call->GetArguments().at(1)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(1).get())->GetIdentifier(), "otherArg");
  EXPECT_EQ(call->GetArguments().at(2)->GetKind(), AST_FUNCTION_ARGUMENT);
  EXPECT_EQ(dynamic_cast<Argument*>(call->GetArguments().at(2).get())->GetIdentifier(), "ok");
  EXPECT_EQ(call->GetIdentifier(), "test");
}

TEST(Call, NamedArgumentMissingIdentifier) {
  // GIVEN
  std::string code = "test(: 40);";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 6};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Call, NamedArgumentMissingColon) {
  // GIVEN
  std::string code = "test(arg 40);";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_TOKEN, "40");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 10};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Call, NamedArgumentMissingValue) {
  // GIVEN
  std::string code = "test(arg:);";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 10};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Call, TrailingComma) {
  // GIVEN
  std::string code = "test(10,);";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 9};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Call, MissingOpeningParenthesis) {
  // GIVEN
  std::string code = "test 10);";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "10");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 6};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

//TODO: Ideally, we want this error message to be expected closing delimiter
TEST(Call, MissingClosingParenthesis) {
  // GIVEN
  std::string code = "test(10;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_TOKEN, ";");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 8};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}