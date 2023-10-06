//
// Created by emma on 25/09/23.
//

#include "compiler.hpp"
#include "../parser/parser.hpp"
#include "../analyzer/semanticValidator.hpp"
#include "../analyzer/type/typeChecker.hpp"
#include "../codegen/codegen.hpp"
#include "../lexer/lexer.hpp"
#include "../utils/stringUtils.hpp"
#include "../utils/fileUtils.hpp"

void
Compiler::CheckForErrors(const std::vector<Errors::Error>& errors) {
  if (!errors.empty()) {
    for (const Errors::Error& error : errors) {
      error.Throw();
    }

    exit(1);
  }
}

std::optional<std::vector<Token>>
Compiler::Lex(const std::string& code, const std::vector<std::string>& codeLines) {
  Lexer lexer{code, codeLines, "/home/emma/Desktop/Kodama/kodama/code.kdm"};
  std::variant<std::vector<Token>, std::vector<Errors::Error>> lexerResult = lexer.Lex();

  if (lexerResult.index() == 0) {
    return get<std::vector<Token>>(lexerResult);
  } else {
    CheckForErrors(get<std::vector<Errors::Error>>(lexerResult));
    return {};
  }
}

std::optional<AstNodePtr>
Compiler::Parse(const std::vector<std::string>& code, const std::vector<Token>& tokens) {
  Parser parser{code, tokens};
  std::variant<AstNodePtr, std::vector<Errors::Error>> parseResult = parser.Parse();

  if (parseResult.index() == 0) {
    return get<AstNodePtr>(parseResult);
  } else {
    CheckForErrors(get<std::vector<Errors::Error>>(parseResult));
    return {};
  }
}

void
Compiler::ValidateSemantics(const std::vector<std::string>& code,
                            const std::vector<Token>& tokens,
                            const AstNodePtr& ast) {
  SemanticValidator semanticValidator{code, tokens};
  std::vector<Errors::Error> semanticErrors = semanticValidator.Validate(ast);
  CheckForErrors(semanticErrors);
}

void
Compiler::TypeCheck(const std::vector<std::string>& code, const std::vector<Token>& tokens, const AstNodePtr& ast) {
  TypeChecker typeChecker{code, tokens};
  std::vector<Errors::Error> typeErrors = typeChecker.TypeCheck(ast);
  CheckForErrors(typeErrors);
}

void
Compiler::GenerateCode(const AstNodePtr& ast, const cli::CliState& state) {
  Codegen codegen{state.skipOptimizations};
  codegen.Generate(ast);

  if (state.emitLLVM) {
    codegen.EmitLLVMIR(state.outputFileName);
  } else {
    codegen.EmitObjectCode(state.outputFileName);
  }
}

void
Compiler::Compile(const cli::CliState& state) {
  std::string program = ReadFile(state.inputFileName);
  std::vector<std::string> codeLines = SplitString(program, "\n");

  std::vector<Token> tokens = Lex(program, codeLines).value();
  AstNodePtr ast = Parse(codeLines, tokens).value();
  //ValidateSemantics(codeLines, tokens, ast);
  //TypeCheck(codeLines, tokens, ast);
  GenerateCode(ast, state);
}