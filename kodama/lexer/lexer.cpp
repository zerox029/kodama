//
// Created by emma on 10/09/23.
//

#include "lexer.hpp"
#include "../errors/error.hpp"
#include <sstream>
#include <unordered_map>
#include <iostream>

Token
Lexer::Peek() {
  bool isLexingStringOldValue = isLexingString;
  bool lastTokenIsStringOldValue = lastTokenIsString;

  Token token = Next();
  index -= token.GetStr().length();
  characterLineIndex -= token.GetStr().length();
  lineNumber -= token.GetTokenType() == TK_NEW_LINE ? 1 : 0;

  isLexingString = isLexingStringOldValue;
  lastTokenIsString = lastTokenIsStringOldValue;

  return token;
}

Token
Lexer::Next() {
  // Eliminate white spaces outside of strings and return new lines as tokens
  while (!isLexingString && isspace(static_cast<unsigned char>(input.at(index))) && index < input.length() - 1) {
    // Handle newlines
    if (isNewline()) {
      lineNumber++;
      index++;
      characterLineIndex = 0;

      return Token{TK_NEW_LINE, "\n", {filePath, lineNumber, characterLineIndex}};
    } else {
      characterLineIndex++;
      index++;
    }
  }

  if (isLexingString && !lastTokenIsString) {
    return ReadString();
  }

  // Process symbols
  std::optional<Token> token = ReadSymbol();
  if (token.has_value()) {
    index += token.value().GetStr().length();
    characterLineIndex += token.value().GetStr().length();

    return token.value();
  }

  // Process keywords
  token = ReadKeyword();
  if (token.has_value()) {
    index += token.value().GetStr().length();
    characterLineIndex += token.value().GetStr().length();

    return token.value();
  }

  // Process numbers
  token = ReadNumber();
  if (token.has_value()) return token.value();

  // Process identifiers
  token = ReadIdentifier();
  if (token.has_value()) return token.value();

  if (index == input.length() - 1) {
    return Token{TK_EOF, "", {filePath, lineNumber, characterLineIndex}};
  }

  throw std::invalid_argument("Invalid token");
}

std::optional<Token>
Lexer::ReadSymbol() {
  switch (input.at(index)) {
    case '+':
      return Token{TK_PLUS, "+", {filePath, lineNumber, characterLineIndex}};
    case '-':
      if (input.at(index + 1) == '>') {
        return Token{TK_ARROW, "->", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_MINUS, "-", {filePath, lineNumber, characterLineIndex}};
    case '*':
      return Token{TK_STAR, "*", {filePath, lineNumber, characterLineIndex}};
    case '/':
      return Token{TK_SLASH, "/", {filePath, lineNumber, characterLineIndex}};
    case '=':
      if (input.at(index + 1) == '=') {
        return Token{TK_EQUAL, "==", {filePath, lineNumber, characterLineIndex}};
      } else if (input.at(index + 1) == '>') {
        return Token{TK_DOUBLE_ARROW, "=>", {filePath, lineNumber, characterLineIndex}};

      }

      return Token{TK_ASSIGN, "=", {filePath, lineNumber, characterLineIndex}};
    case '%':
      return Token{TK_PERCENT, "%", {filePath, lineNumber, characterLineIndex}};
    case ':':
      return Token{TK_COLON, ":", {filePath, lineNumber, characterLineIndex}};
    case ';':
      return Token{TK_SEMICOLON, ";", {filePath, lineNumber, characterLineIndex}};
    case '(':
      return Token{TK_OPEN_PAREN, "(", {filePath, lineNumber, characterLineIndex}};
    case ')':
      return Token{TK_CLOSED_PAREN, ")", {filePath, lineNumber, characterLineIndex}};
    case '{':
      return Token{TK_OPEN_CURLY, "{", {filePath, lineNumber, characterLineIndex}};
    case '}':
      return Token{TK_CLOSED_CURLY, "}", {filePath, lineNumber, characterLineIndex}};
    case ',':
      return Token{TK_COMMA, ",", {filePath, lineNumber, characterLineIndex}};
    case '.':
      return Token{TK_DOT, ".", {filePath, lineNumber, characterLineIndex}};
    case '"':
      if (!lastTokenIsString) { //TODO: Find a better way to handle this
        isLexingString = true;
      } else {
        isLexingString = false;
        lastTokenIsString = false;
      }

      return Token{TK_QUOTATION, "\"", {filePath, lineNumber, characterLineIndex}};
    case '>':
      if (input.at(index + 1) == '=') {
        return Token{TK_GREATER_EQ, ">=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_GREATER, ">", {filePath, lineNumber, characterLineIndex}};
    case '<':
      if (input.at(index + 1) == '=') {
        return Token{TK_LESS_EQ, "<=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_LESS, "<", {filePath, lineNumber, characterLineIndex}};
    case '!':
      if (input.at(index + 1) == '=') {
        return Token{TK_NOT_EQUAL, "!=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_BANG, "!", {filePath, lineNumber, characterLineIndex}};
    default:
      return {};
  }
}

bool
Lexer::isNewline() {
  return input.substr(index).starts_with("\r\n")
      || input.substr(index).starts_with('\n')
      || input.substr(index).starts_with('\r');
}

std::optional<Token>
Lexer::ReadKeyword() {
  for (const auto& symbol : keywords) {
    if (input.substr(index).starts_with(symbol.first) && !isalnum(input.at(index + symbol.first.length()))) {
      return Token{symbol.second, symbol.first, {filePath, lineNumber, characterLineIndex}};
    }
  }

  return {};
}

std::optional<Token>
Lexer::ReadNumber() {
  std::stringstream numValue{};

  size_t startingPosition = characterLineIndex;
  while (index < input.length() && isdigit(static_cast<unsigned char>(input.at(index)))) {
    numValue << input.at(index);
    index++;
    characterLineIndex++;
  }

  if (numValue.str().length() == 0)
    return {};

  return Token{TK_NUMBER, numValue.str(), {filePath, lineNumber, startingPosition}};
}

std::optional<Token>
Lexer::ReadIdentifier() {
  std::stringstream identifierValue{};

  size_t startingPosition = characterLineIndex;
  while (index < input.length() && isalpha(static_cast<unsigned char>(input.at(index)))) {
    identifierValue << input.at(index);
    index++;
    characterLineIndex++;
  }

  if (identifierValue.str().length() == 0)
    return {};

  return Token{TK_IDENTIFIER,
               identifierValue.str(),
               {filePath, lineNumber, startingPosition}};
}

Token
Lexer::ReadString() {
  std::stringstream stringValue{};

  //TODO: Allow for escaped quotation marks
  while (index < input.length() && input.at(index) != '\"') {
    stringValue << input.at(index);
    index++;
  }

  lastTokenIsString = true;

  return Token{TK_STRING,
               stringValue.str(),
               {filePath, lineNumber, characterLineIndex - stringValue.str().length()}};
}

std::vector<Token>
Lexer::Tokenize() {
  std::vector<Token> tokens{};

  while (index < input.length() && Peek().GetTokenType() != TK_EOF) {
    tokens.push_back(Next());
  }

  tokens.emplace_back(Token{TK_EOF, "", {filePath, lineNumber, characterLineIndex}});

  return tokens;
}