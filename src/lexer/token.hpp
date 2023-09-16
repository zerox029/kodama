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
  TK_COLON,
  TK_ASSIGN,

  TK_LET,

  TK_I8,
  TK_I16,
  TK_I32,
  TK_I64,
  TK_I128,

  TK_NUMBER,
  TK_LABEL,

  TK_EOF
};

class Token {
 public:
  Token(TokenType type, std::string  str) : type{type}, str{std::move(str)} {}

  [[nodiscard]] TokenType getTokenType() const { return type; }
  [[nodiscard]] std::string getStr() const { return str; }

 private:
  TokenType type;
  std::string str;
};

#endif //KODAMA_SRC_LEXER_TOKEN_HPP_
