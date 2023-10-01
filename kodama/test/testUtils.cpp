//
// Created by emma on 29/09/23.
//

#include "testUtils.hpp"
#include "../analyzer/semanticValidator.hpp"

AstNodePtr
buildAST(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, codeLines, DUMMY_FILE_LOCATION};
  std::variant<std::vector<Token>, std::vector<Errors::Error>> lexerResult = lexer.Lex();

  Parser parser{codeLines, get<std::vector<Token>>(lexerResult)};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();

  return get<AstNodePtr>(parseResult);
}

std::vector<Errors::Error>
getErrorsForAst(const std::string& code) {
  std::vector<std::string> codeLines = SplitString(code, "\n");

  Lexer lexer{code, codeLines, DUMMY_FILE_LOCATION};
  std::variant<std::vector<Token>, std::vector<Errors::Error>> lexerResult = lexer.Lex();

  Parser parser{codeLines, get<std::vector<Token>>(lexerResult)};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();

  return get<std::vector<Errors::Error>>(parseResult);
}

std::vector<Errors::Error>
GetSemanticErrors(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast) {
  SemanticValidator semanticValidator{code, tokens};
  std::vector<Errors::Error> semanticErrors = semanticValidator.Validate(ast);

  return semanticErrors;
}

void AssertEqLocation(const Location& expected, const Location& actual) {
  EXPECT_EQ(expected.lineNumber, actual.lineNumber);
  EXPECT_EQ(expected.characterLineIndex, actual.characterLineIndex);
  EXPECT_EQ(expected.filePath, actual.filePath);
}