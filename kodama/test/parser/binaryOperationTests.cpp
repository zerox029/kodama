//
// Created by emma on 30/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(BinaryOperation, EqualHappyPath) {
  // GIVEN
  std::string code = "10 == 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_EQUAL);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, NotEqualHappyPath) {
  // GIVEN
  std::string code = "10 != 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_NOT_EQUAL);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, GreaterHappyPath) {
  // GIVEN
  std::string code = "10 > 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_GREATER);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, LessHappyPath) {
  // GIVEN
  std::string code = "10 < 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_LESS);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, GreaterEqualHappyPath) {
  // GIVEN
  std::string code = "10 >= 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_GREATER_EQ);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, LessEqualHappyPath) {
  // GIVEN
  std::string code = "10 <= 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_LESS_EQ);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, AddHappyPath) {
  // GIVEN
  std::string code = "10 + 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_PLUS);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, SubHappyPath) {
  // GIVEN
  std::string code = "10 - 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_MINUS);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, MulHappyPath) {
  // GIVEN
  std::string code = "10 * 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_STAR);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, DivisionHappyPath) {
  // GIVEN
  std::string code = "10 / 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_SLASH);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, ModHappyPath) {
  // GIVEN
  std::string code = "10 % 20;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* binaryNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(binaryNode->GetOperator().GetTokenType(), TK_PERCENT);
  EXPECT_EQ(binaryNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(binaryNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(BinaryOperation, MissingRhsAdd) {
  // GIVEN
  std::string code = "10 +;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 5};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BinaryOperation, MissingRhsMul) {
  // GIVEN
  std::string code = "10 /;";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_VALUE_IDENTIFIER);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 5};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BinaryOperation, MissingRhsEq) {
  // GIVEN
  std::string code = "10 ==;";
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

TEST(BinaryOperation, MissingLhsMul) {
  // GIVEN
  std::string code = "/ 5;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_TOKEN, "/");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 1};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BinaryOperation, MissingLhsEq) {
  // GIVEN
  std::string code = "== 5;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_TOKEN, "==");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 1};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BinaryOperation, MissingOperator) {
  // GIVEN
  std::string code = "6 5;";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "5");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 3};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(OrderOperations, AddbeforeEquality) {
  // GIVEN
  std::string code = "10 == 6 + 7;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* equalityNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());
  auto* addNode = dynamic_cast<BinaryOperation*>(equalityNode->GetRhs().get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(equalityNode->GetOperator().GetTokenType(), TK_EQUAL);
  EXPECT_EQ(equalityNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(equalityNode->GetRhs()->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(addNode->GetOperator().GetTokenType(), TK_PLUS);
  EXPECT_EQ(addNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(addNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(OrderOperations, MulBeforeAdd) {
  // GIVEN
  std::string code = "10 + 6 * 7;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* plusNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());
  auto* mulNode = dynamic_cast<BinaryOperation*>(plusNode->GetRhs().get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(plusNode->GetOperator().GetTokenType(), TK_PLUS);
  EXPECT_EQ(plusNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(plusNode->GetRhs()->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(mulNode->GetOperator().GetTokenType(), TK_STAR);
  EXPECT_EQ(mulNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(mulNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(OrderOperations, RecursiveOperation) {
  // GIVEN
  std::string code = "10 + 9 + 8;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* plusNode = dynamic_cast<BinaryOperation*>(program->GetStatements().at(0).get());
  auto* innerPlusNode = dynamic_cast<BinaryOperation*>(plusNode->GetRhs().get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(plusNode->GetOperator().GetTokenType(), TK_PLUS);
  EXPECT_EQ(plusNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(plusNode->GetRhs()->GetKind(), AST_BINARY_OPERATION);
  EXPECT_EQ(innerPlusNode->GetOperator().GetTokenType(), TK_PLUS);
  EXPECT_EQ(innerPlusNode->GetLhs()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(innerPlusNode->GetRhs()->GetKind(), AST_INTEGER_LITERAL);
}