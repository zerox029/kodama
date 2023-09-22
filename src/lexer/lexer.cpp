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

  isLexingString = isLexingStringOldValue;
  lastTokenIsString = lastTokenIsStringOldValue;

  return token;
}

Token
Lexer::Next() {
  // Eliminate white spaces outside of strings
  while (!isLexingString && isspace(static_cast<unsigned char>(input.at(index))) && index < input.length() - 1) {
    // Handle newlines
    if (isNewline()) {
      lineNumber++;
      characterLineIndex = 0;
    } else {
      characterLineIndex++;
    }

    index++;
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
  for (const auto& symbol : symbols) {
    if (input.substr(index).starts_with(symbol.first)) {
      if (symbol.second == TK_QUOTATION && !lastTokenIsString) { //TODO: Find a better way to handle this
        isLexingString = true;
      } else if (symbol.second == TK_QUOTATION && lastTokenIsString) {
        isLexingString = false;
        lastTokenIsString = false;
      }

      return Token{symbol.second, symbol.first, {filePath, lineNumber, characterLineIndex}};
    }
  }

  return {};
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
    if (input.substr(index).starts_with(symbol.first)) {
      return Token{symbol.second, symbol.first, {filePath, lineNumber, characterLineIndex}};
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
    characterLineIndex++;
  }

  if (numValue.str().length() == 0)
    return {};

  return Token{TK_NUMBER, numValue.str(), {filePath, lineNumber, characterLineIndex}};
}

std::optional<Token>
Lexer::ReadIdentifier() {
  std::stringstream identifierValue{};

  while (index < input.length() && isalpha(static_cast<unsigned char>(input.at(index)))) {
    identifierValue << input.at(index);
    index++;
    characterLineIndex++;
  }

  if (identifierValue.str().length() == 0)
    return {};

  return Token{TK_IDENTIFIER,
               identifierValue.str(),
               {filePath, lineNumber, characterLineIndex - identifierValue.str().length()}};
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

std::string
Lexer::GetSymbolFromTokenType(TokenType tokenType) {
  for (const auto& symbol : symbols) {
    if (symbol.second == tokenType) return symbol.first;
  }

  return "";
}