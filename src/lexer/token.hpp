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

  TK_NUMBER,

  TK_EOF
};

class Token {
 public:
  Token(TokenType type, const std::string& str) : type{type}, str{str} {}

  TokenType getTokenType() { return type; }
  std::string_view getStr() { return str; }

 private:
  TokenType type;
  std::string_view str;
};

#endif //KODAMA_SRC_LEXER_TOKEN_HPP_
