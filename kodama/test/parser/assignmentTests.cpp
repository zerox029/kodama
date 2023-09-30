//
// Created by emma on 30/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(Assignment, LetHappyPath) {
  // GIVEN
  std::string code = "let i: i64 = 10;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* assignmentNode = dynamic_cast<AssignmentExpression*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_ASSIGNMENT);
  EXPECT_EQ(assignmentNode->GetIdentifier(), "i");
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeCategory(), INTEGER);
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeName(), I64_TYPE);
  EXPECT_EQ(assignmentNode->GetDataType()->IsMutable(), true);
  EXPECT_EQ(assignmentNode->GetValue()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(Assignment, ValHappyPath) {
  // GIVEN
  std::string code = "val i: i64 = 10;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* assignmentNode = dynamic_cast<AssignmentExpression*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_ASSIGNMENT);
  EXPECT_EQ(assignmentNode->GetIdentifier(), "i");
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeCategory(), INTEGER);
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeName(), I64_TYPE);
  EXPECT_EQ(assignmentNode->GetDataType()->IsMutable(), false);
  EXPECT_EQ(assignmentNode->GetValue()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(Assignment, HappyPathNoValue) {
  // GIVEN
  std::string code = "let i: i64;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* assignmentNode = dynamic_cast<AssignmentExpression*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_ASSIGNMENT);
  EXPECT_EQ(assignmentNode->GetIdentifier(), "i");
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeCategory(), INTEGER);
  EXPECT_EQ(assignmentNode->GetDataType()->GetTypeName(), I64_TYPE);
  EXPECT_EQ(assignmentNode->GetDataType()->IsMutable(), true);
  EXPECT_EQ(assignmentNode->GetValue(), nullptr);
}

TEST(Assignment, MissingAssignmentKeyword) {
  // GIVEN
  std::string code = "i: i64 = 10;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, ":");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 2};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Assignment, MissingColonKeyword) {
  // GIVEN
  std::string code = "let i i64 = 10;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_TOKEN, ":", "i64");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 7};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Assignment, MissingDataType) {
  // GIVEN
  std::string code = "let i: = 10;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_DATATYPE);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 8};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Assignment, MissingAssignmentOperator) {
  // GIVEN
  std::string code = "let i: i32 10;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "10");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 12};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Assignment, MissingValue) {
  // GIVEN
  std::string code = "let i: i32 =;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 13};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Reassignment, HappyPath) {
  // GIVEN
  std::string code = "x = 70;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* reassignmentNode = dynamic_cast<ReassignmentExpression*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_REASSIGNMENT);
  EXPECT_EQ(reassignmentNode->GetIdentifier(), "x");
  EXPECT_EQ(reassignmentNode->GetValue()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(Reassignment, MissingIdentifier) {
  // GIVEN
  std::string code = "= 7";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "=");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 1};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Reassignment, MissingOperator) {
  // GIVEN
  std::string code = "x 7;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "7");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 3};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(Reassignment, MissingValue) {
  // GIVEN
  std::string code = "x =;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 4};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}