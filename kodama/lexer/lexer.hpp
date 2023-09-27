//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_LEXER_HPP_
#define KODAMA_SRC_LEXER_LEXER_HPP_

#include "token.hpp"
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <utility>
#include <unordered_map>

const std::unordered_map<std::string, TokenType> symbols = {
    {"==", TK_EQUAL}, {"!=", TK_NOT_EQUAL}, {"->", TK_ARROW},
    {"=>", TK_DOUBLE_ARROW}, {">=", TK_GREATER_EQ}, {"<=", TK_LESS_EQ},
    {"+", TK_PLUS}, {"-", TK_MINUS}, {"*", TK_STAR}, {"/", TK_SLASH},
    {"=", TK_ASSIGN}, {"%", TK_PERCENT}, {":", TK_COLON}, {";", TK_SEMICOLON},
    {"(", TK_OPEN_PAREN}, {")", TK_CLOSED_PAREN}, {"{", TK_OPEN_CURLY},
    {"}", TK_CLOSED_CURLY}, {",", TK_COMMA}, {".", TK_DOT},
    {"\"", TK_QUOTATION}, {">", TK_GREATER}, {"<", TK_LESS}
};

const std::unordered_map<std::string, TokenType> keywords = {
    {"u8", TK_U8}, {"u16", TK_U16}, {"u32", TK_U32},
    {"u64", TK_U64}, {"u128", TK_U128}, {"i8", TK_I8},
    {"i16", TK_I16}, {"i32", TK_I32}, {"i64", TK_I64},
    {"i128", TK_I128}, {"f32", TK_F32}, {"f64", TK_F64},
    {"let", TK_LET}, {"return", TK_RET}, {"if", TK_IF},
    {"else", TK_ELSE}, {"do", TK_DO}, {"while", TK_WHILE},
    {"for", TK_FOR}, {"in", TK_IN}, {"def", TK_DEF},
    {"extern", TK_EXTERN}, {"bool", TK_BOOL}, {"null", TK_NULL},
    {"true", TK_TRUE}, {"false", TK_FALSE}, {"string", TK_STRING},
    {"val", TK_VAL}
};

class Lexer {
 public:
  explicit Lexer(std::string input, std::string filePath) : input{std::move(input)}, filePath{std::move(filePath)} {}

  std::vector<Token> Tokenize();
  static std::string GetSymbolFromTokenType(TokenType tokenType);

 private:
  std::string input;
  size_t index = 0;

  std::string filePath;
  size_t lineNumber = 0;
  size_t characterLineIndex = 0;

  bool isLexingString = false;
  bool lastTokenIsString = false;

  Token Peek();
  Token Next();
  bool isNewline();
  std::optional<Token> ReadSymbol();
  std::optional<Token> ReadKeyword();
  std::optional<Token> ReadNumber();
  std::optional<Token> ReadIdentifier();
  Token ReadString();
};

#endif //KODAMA_SRC_LEXER_LEXER_HPP_
