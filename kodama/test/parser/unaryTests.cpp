//
// Created by emma on 30/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(Unary, PlusHappyPath) {
  // GIVEN
  std::string code = "+10;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* integerNode = dynamic_cast<IntegerLiteral*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(integerNode->GetValue(), "10");
}

TEST(Unary, MinusHappyPath) {
  // GIVEN
  std::string code = "-10;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_MINUS);
  EXPECT_EQ(dynamic_cast<IntegerLiteral*>(binaryNode->GetLhs().get())->GetValue(), "0");
  EXPECT_EQ(dynamic_cast<IntegerLiteral*>(binaryNode->GetRhs().get())->GetValue(), "10");
}

TEST(BinaryOperation, PlusMissingValue) {
  // GIVEN
  std::string code = "+;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 2};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BinaryOperation, MinusMissingValue) {
  // GIVEN
  std::string code = "-;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 2};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}