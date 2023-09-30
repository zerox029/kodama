//
// Created by emma on 29/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(ReturnStatement, HappyPath) {
  // GIVEN
  std::string code = "return 10;";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* returnNode = dynamic_cast<ReturnStatement*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_RETURN);
  EXPECT_EQ(dynamic_cast<IntegerLiteral*>(returnNode->GetReturnValue().get())->GetValue(), "10");
}

TEST(ReturnStatement, UnexpectedExpression) {
  // GIVEN
  std::string code = "return 10 return 3";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "return");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 11};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(), expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(BlockStatement, HappyPath) {
  // GIVEN
  std::string code = "{ return 10; }";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* blockNode = dynamic_cast<Block*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_BLOCK);
  EXPECT_EQ(blockNode->GetStatements().size(), 1);
}

TEST(IfStatement, HappyPath) {
  // GIVEN
  std::string code = "if(true) { return 10; }";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* ifNode = dynamic_cast<IfStatement*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_IF);
  EXPECT_EQ(ifNode->GetCondition()->GetKind(), AST_BOOL_VALUE);
  EXPECT_EQ(ifNode->GetConsequent()->GetKind(), AST_BLOCK);
}

TEST(IfStatement, MissingCondition) {
  // GIVEN
  std::string code = "if() {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_CONDITION);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 4};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(IfElseStatement, HappyPath) {
  // GIVEN
  std::string code = "if(true) { return 10; } else { return 20; }";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* ifElseNode = dynamic_cast<IfElseStatement*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_IF_ELSE);
  EXPECT_EQ(ifElseNode->GetCondition()->GetKind(), AST_BOOL_VALUE);
  EXPECT_EQ(ifElseNode->GetConsequent()->GetKind(), AST_BLOCK);
  EXPECT_EQ(ifElseNode->GetAlternative()->GetKind(), AST_BLOCK);
}

TEST(IfElseStatement, IfElseIfHappyPath) {
  // GIVEN
  std::string code = "if(true) { return 10; } else if(true) { return 20; }";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* ifElseNode = dynamic_cast<IfElseStatement*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_IF_ELSE);
  EXPECT_EQ(ifElseNode->GetCondition()->GetKind(), AST_BOOL_VALUE);
  EXPECT_EQ(ifElseNode->GetConsequent()->GetKind(), AST_BLOCK);
  EXPECT_EQ(ifElseNode->GetAlternative()->GetKind(), AST_IF);
}

TEST(WhileLoop, HappyPath) {
  // GIVEN
  std::string code = "while(true) {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* whileNode = dynamic_cast<WhileLoop*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_WHILE);
  EXPECT_EQ(whileNode->GetCondition()->GetKind(), AST_BOOL_VALUE);
  EXPECT_EQ(whileNode->GetConsequent()->GetKind(), AST_BLOCK);
}

TEST(WhileLoop, MissingCondition) {
  // GIVEN
  std::string code = "while() {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_CONDITION);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 7};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(DoWhileLoop, HappyPath) {
  // GIVEN
  std::string code = "do {} while(true) {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* doWhileNode = dynamic_cast<DoWhileLoop*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_DO_WHILE);
  EXPECT_EQ(doWhileNode->GetCondition()->GetKind(), AST_BOOL_VALUE);
  EXPECT_EQ(doWhileNode->GetConsequent()->GetKind(), AST_BLOCK);
}

TEST(DoWhileLoop, MissingCondition) {
  // GIVEN
  std::string code = "do {} while() {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_CONDITION);
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 13};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(DoWhileLoop, MissingWhile) {
  // GIVEN
  std::string code = "do {} () {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_KEYWORD, "while", "(");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 7};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(DoWhileLoop, MissingWhileOpParen) {
  // GIVEN
  std::string code = "do {} while true) {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_OP_DELIMITER, "(", "true");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 13};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(DoWhileLoop, MissingWhileClParen) {
  // GIVEN
  std::string code = "do {} while(true {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_CL_DELIMITER, ")", "{");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 18};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(ForLoop, HappyPathTo) {
  // GIVEN
  std::string code = "for i in 0 to 10 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* forNode = dynamic_cast<ForLoop*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FOR);
  EXPECT_EQ(forNode->GetIdentifier(), "i");
  EXPECT_EQ(forNode->GetFrom()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(forNode->GetTo()->GetKind(), AST_INTEGER_LITERAL);
}

TEST(ForLoop, HappyPathUntil) {
  // GIVEN
  std::string code = "for i in 0 until 10 {}";

  // WHEN
  AstNodePtr ast = buildAST(code);
  auto* program = dynamic_cast<Program*>(ast.get());
  auto* forNode = dynamic_cast<ForLoop*>(program->GetStatements().at(0).get());

  // THEN
  EXPECT_EQ(program->GetStatements().at(0)->GetKind(), AST_FOR);
  EXPECT_EQ(forNode->GetIdentifier(), "i");
  EXPECT_EQ(forNode->GetFrom()->GetKind(), AST_INTEGER_LITERAL);
  EXPECT_EQ(forNode->GetTo()->GetKind(), AST_BINARY_OPERATION);
}

TEST(IfStatement, MissingIdentifier) {
  // GIVEN
  std::string code = "for in 0 until 10 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_IDENTIFIER, "in");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 5};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(IfStatement, MissingFrom) {
  // GIVEN
  std::string code = "for i in until 10 {}";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "until");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 10};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(IfStatement, MissingTo) {
  // GIVEN
  std::string code = "for i in 0 until {}";
  std::string expectedMessage = std::format(Errors::Strings::UNEXPECTED_EXPRESSION, "{");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 18};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}

TEST(IfStatement, MissingToUntilKeyword) {
  // GIVEN
  std::string code = "for i in 0 10 {}";
  std::string expectedMessage = std::format(Errors::Strings::EXPECTED_TO_UNTIL, "10");
  Location expectedLocation{DUMMY_FILE_LOCATION, 1, 12};

  // WHEN
  std::vector<Error> errors = getErrorsForAst(code);

  // THEN
  EXPECT_EQ(errors.size(), 1);
  EXPECT_EQ(errors.at(0).GetErrorClass(), "syntax error");
  EXPECT_EQ(errors.at(0).GetErrorMessage(),  expectedMessage);
  AssertEqLocation(expectedLocation, errors.at(0).GetErrorLocation());
}