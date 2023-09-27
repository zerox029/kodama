//
// Created by emma on 10/09/23.
//

#ifndef KODAMA_SRC_LEXER_TOKEN_HPP_
#define KODAMA_SRC_LEXER_TOKEN_HPP_

#include <string>
#include <string_view>
#include <utility>

enum TokenType {
  // Operators
  TK_PLUS,        // +
  TK_MINUS,       // -
  TK_STAR,        // *
  TK_SLASH,       // /
  TK_PERCENT,     // %
  TK_ASSIGN,      // =
  TK_EQUAL,       // ==
  TK_NOT_EQUAL,   // !=
  TK_GREATER,     // >
  TK_LESS,        // <
  TK_GREATER_EQ,  // >=
  TK_LESS_EQ,     // <=
  TK_COLON,       // :
  TK_SEMICOLON,   // ;
  TK_ARROW,       // ->
  TK_DOUBLE_ARROW,// =>
  TK_QUOTATION,   // "
  TK_COMMA,       // ,
  TK_DOT,         // .

  TK_OPEN_PAREN,  // (
  TK_CLOSED_PAREN,// )
  TK_OPEN_CURLY,  // {
  TK_CLOSED_CURLY,// }

  // Data types
  TK_U8,          // u8
  TK_U16,         // u16
  TK_U32,         // u32
  TK_U64,         // u64
  TK_U128,        // u128
  TK_I8,          // i8
  TK_I16,         // i16
  TK_I32,         // i32
  TK_I64,         // i64
  TK_I128,        // i128
  TK_F32,         // f32
  TK_F64,         // f64
  TK_BOOL,        // bool
  TK_STRING,      // string

  // Reserved keywords
  TK_LET,         // let
  TK_VAL,         // val
  TK_RET,         // return
  TK_DEF,         // def
  TK_EXTERN,      // extern
  TK_IF,          // if
  TK_ELSE,        // else
  TK_DO,          // do
  TK_WHILE,       // while
  TK_FOR,         // for
  TK_IN,          // in
  TK_NULL,        // null
  TK_TRUE,        // true
  TK_FALSE,       // false

  // Value types
  TK_NUMBER,
  TK_IDENTIFIER,

  TK_EOF          // end of file
};

struct Location {
  std::string filePath;
  size_t lineNumber;
  size_t characterLineIndex;
};

class Token {
 public:
  Token(TokenType type, std::string str, Location location) : type{type}, str{std::move(str)}, location{std::move(location)} {}

  [[nodiscard]] TokenType GetTokenType() const { return type; }
  [[nodiscard]] std::string GetStr() const { return str; }
  [[nodiscard]] Location GetLocation() const { return location; }

 private:
  TokenType type;
  std::string str;
  Location location;
};

#endif //KODAMA_SRC_LEXER_TOKEN_HPP_
