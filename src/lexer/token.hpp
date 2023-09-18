//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_TOKEN_HPP_
#define KODAMA_SRC_LEXER_TOKEN_HPP_

#include <string>
#include <string_view>
#include <utility>

enum TokenType {
  TK_PLUS,
  TK_MINUS,
  TK_STAR,
  TK_SLASH,
  TK_PERCENT,
  TK_ASSIGN,
  TK_COLON,
  TK_SEMICOLON,
  TK_ARROW,

  TK_COMMA,

  TK_OPEN_PAREN,
  TK_CLOSED_PAREN,
  TK_OPEN_CURLY,
  TK_CLOSED_CURLY,

  TK_EQUAL,
  TK_NOT_EQUAL,

  TK_LET,
  TK_RET,
  TK_FN,

  TK_U8,
  TK_U16,
  TK_U32,
  TK_U64,
  TK_U128,
  TK_I8,
  TK_I16,
  TK_I32,
  TK_I64,
  TK_I128,

  TK_IF,
  TK_ELSE,
  TK_DO,
  TK_WHILE,
  TK_FOR,
  TK_IN,

  TK_NUMBER,
  TK_IDENTIFIER,

  TK_EOF
};

class Token {
 public:
  Token(TokenType type, std::string str) : type{type}, str{std::move(str)} {}

  [[nodiscard]] TokenType getTokenType() const { return type; }
  [[nodiscard]] std::string getStr() const { return str; }

 private:
  TokenType type;
  std::string str;
};

#endif //KODAMA_SRC_LEXER_TOKEN_HPP_
