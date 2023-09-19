//
// Created by emma on 10/09/23.
//

#include "lexer.hpp"
#include "../errors/error.hpp"
#include <sstream>
#include <unordered_map>
#include <iostream>

const std::unordered_map<std::string, TokenType> dualCharacterSymbols = {
    {"==", TK_EQUAL}, {"!=", TK_NOT_EQUAL}, {"->", TK_ARROW}
};

const std::unordered_map<std::string, TokenType> singleCharacterSymbols = {
    {"+", TK_PLUS}, {"-", TK_MINUS}, {"*", TK_STAR}, {"/", TK_SLASH},
    {"=", TK_ASSIGN}, {"%", TK_PERCENT}, {":", TK_COLON}, {";", TK_SEMICOLON},
    {"(", TK_OPEN_PAREN}, {")", TK_CLOSED_PAREN}, {"{", TK_OPEN_CURLY},
    {"}", TK_CLOSED_CURLY}, {",", TK_COMMA}, {".", TK_DOT}
};

const std::unordered_map<std::string, TokenType> keywords = {
    {"u8", TK_U8}, {"u16", TK_U16}, {"u32", TK_U32},
    {"u64", TK_U64}, {"u128", TK_U128}, {"i8", TK_I8},
    {"i16", TK_I16}, {"i32", TK_I32}, {"i64", TK_I64},
    {"i128", TK_I128}, {"f32", TK_F32}, {"f64", TK_F64},
    {"let", TK_LET}, {"return", TK_RET}, {"if", TK_IF},
    {"else", TK_ELSE}, {"do", TK_DO}, {"while", TK_WHILE},
    {"for", TK_FOR}, {"in", TK_IN}, {"fn", TK_FN}
};

Token
Lexer::Peek() {
  Token token = Next();
  index -= token.getStr().length();

  return token;
}

Token
Lexer::Next() {
  // Eliminate white spaces
  while (isspace(static_cast<unsigned char>(input.at(index)))) {
    index++;
  }

  // Process symbols
  std::optional<Token> token = ReadSymbol();
  if (token.has_value()) {
    index += token.value().getStr().length();
    return token.value();
  }

  // Process keywords
  token = ReadKeyword();
  if (token.has_value()) {
    index += token.value().getStr().length();
    return token.value();
  }

  // Process numbers
  token = ReadNumber();
  if (token.has_value()) return token.value();

  // Process identifiers
  token = ReadIdentifier();
  if (token.has_value()) return token.value();

  Error error{TOKENIZATION_ERROR, {0, 0}};
  error.Throw(TOKENIZATION_ERROR);

  throw std::invalid_argument("Invalid token");
}

std::optional<Token>
Lexer::ReadSymbol() {
  for (const auto& symbol : dualCharacterSymbols) {
    if (input.substr(index).starts_with(symbol.first)) {
      return Token{symbol.second, symbol.first};
    }
  }

  for (const auto& symbol : singleCharacterSymbols) {
    if (input.substr(index).starts_with(symbol.first)) {
      return Token{symbol.second, symbol.first};
    }
  }

  return {};
}

std::optional<Token>
Lexer::ReadKeyword() {
  for (const auto& symbol : keywords) {
    if (input.substr(index).starts_with(symbol.first)) {
      return Token{symbol.second, symbol.first};
    }
  }

  return {};
}

std::optional<Token>
Lexer::ReadNumber() {
  std::stringstream numValue{};

  while (index < input.length() && isdigit(static_cast<unsigned char>(input.at(index)))) {
    numValue << input.at(index);
    index++;
  }

  if (numValue.str().length() == 0)
    return {};

  return Token{TK_NUMBER, numValue.str()};
}

std::optional<Token>
Lexer::ReadIdentifier() {
  std::stringstream identifierValue{};

  while (index < input.length() && isalpha(static_cast<unsigned char>(input.at(index)))) {
    identifierValue << input.at(index);
    index++;
  }

  if (identifierValue.str().length() == 0)
    return {};

  return Token{TK_IDENTIFIER, identifierValue.str()};
}

std::vector<Token>
Lexer::Tokenize() {
  std::vector<Token> tokens{};

  while (index < input.length() && Peek().getTokenType() != TK_EOF) {
    tokens.push_back(Next());
  }

  return tokens;
}