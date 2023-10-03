//
// Created by emma on 10/09/23.
//

#include "lexer.hpp"
#include <sstream>
#include <unordered_map>
#include <iostream>

// TODO: Find a better way to handle this
Token
Lexer::Peek() {
  bool isLexingStringOldValue = isLexingString;
  bool lastTokenIsStringOldValue = lastTokenIsString;
  std::stack<Token> oldUnclosedDelimiters = unclosedDelimiters;

  Token token = Next();
  index -= token.GetStr().length();
  characterLineIndex -= token.GetStr().length();
  lineNumber -= token.GetTokenType() == TK_NEW_LINE ? 1 : 0;

  isLexingString = isLexingStringOldValue;
  lastTokenIsString = lastTokenIsStringOldValue;
  unclosedDelimiters = oldUnclosedDelimiters;

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
      if (input.at(index + 1) == '=') {
        return Token{TK_PLUS_ASSIGN, "+=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_PLUS, "+", {filePath, lineNumber, characterLineIndex}};
    case '-':
      if (input.at(index + 1) == '=') {
        return Token{TK_MINUS_ASSIGN, "-=", {filePath, lineNumber, characterLineIndex}};
      }
      else if (input.at(index + 1) == '>') {
        return Token{TK_ARROW, "->", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_MINUS, "-", {filePath, lineNumber, characterLineIndex}};
    case '*':
      if (input.at(index + 1) == '=') {
        return Token{TK_STAR_ASSIGN, "*=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_STAR, "*", {filePath, lineNumber, characterLineIndex}};
    case '/':
      if (input.at(index + 1) == '=') {
        return Token{TK_SLASH_ASSIGN, "/=", {filePath, lineNumber, characterLineIndex}};
      }
      else if(input.at(index + 1) == '/') {
        Token commentToken{TK_COMMENT, "//", {filePath, lineNumber, characterLineIndex}};
        SkipCurrentLine();

        return commentToken;
      }

      return Token{TK_SLASH, "/", {filePath, lineNumber, characterLineIndex}};
    case '=':
      if (input.at(index + 1) == '=') {
        return Token{TK_EQUAL, "==", {filePath, lineNumber, characterLineIndex}};
      } else if (input.at(index + 1) == '>') {
        return Token{TK_DOUBLE_ARROW, "=>", {filePath, lineNumber, characterLineIndex}};

      }

      return Token{TK_ASSIGN, "=", {filePath, lineNumber, characterLineIndex}};
    case '%':
      if (input.at(index + 1) == '=') {
        return Token{TK_MOD_ASSIGN, "%=", {filePath, lineNumber, characterLineIndex}};
      }

      return Token{TK_PERCENT, "%", {filePath, lineNumber, characterLineIndex}};
    case ':':
      return Token{TK_COLON, ":", {filePath, lineNumber, characterLineIndex}};
    case ';':
      return Token{TK_SEMICOLON, ";", {filePath, lineNumber, characterLineIndex}};
    case '(': {
      Token token = Token{TK_OPEN_PAREN, "(", {filePath, lineNumber, characterLineIndex}};
      unclosedDelimiters.push(token);

      return token;
    }
    case ')': {
      if(!unclosedDelimiters.empty() && unclosedDelimiters.top().GetTokenType() == TK_OPEN_PAREN) {
        unclosedDelimiters.pop();
      }

      return Token{TK_CLOSED_PAREN, ")", {filePath, lineNumber, characterLineIndex}};
    }
    case '{': {
      Token token{TK_OPEN_CURLY, "{", {filePath, lineNumber, characterLineIndex}};
      unclosedDelimiters.push(token);

      return token;
    }
    case '}': {
      if(!unclosedDelimiters.empty() && unclosedDelimiters.top().GetTokenType() == TK_OPEN_CURLY) {
        unclosedDelimiters.pop();
      }

      return Token{TK_CLOSED_CURLY, "}", {filePath, lineNumber, characterLineIndex}};
    }
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
  while (index <= input.length() && input.at(index) != '\"') {
    if (index == input.length() - 1) {
      Location startLocation = {filePath, lineNumber, characterLineIndex - stringValue.str().length() - 1};
      LogError(Errors::UNTERMINATED_STRING, startLocation);

      return Token{TK_EOF, "", startLocation};
    }

    stringValue << input.at(index);
    index++;
    characterLineIndex++;
  }

  lastTokenIsString = true;

  return Token{TK_STRING,
               stringValue.str(),
               {filePath, lineNumber, characterLineIndex - stringValue.str().length()}};
}

void
Lexer::SkipCurrentLine() {
  while(!isNewline()) {
    index++;
  }

  index++;
  lineNumber++;
  characterLineIndex = 0;
}

std::variant<std::vector<Token>, std::vector<Errors::Error>>
Lexer::Lex() {
  std::vector<Token> tokens{};

  while (index < input.length() && Peek().GetTokenType() != TK_EOF) {
    Token nextToken = Next();
    if(nextToken.GetTokenType() != TK_COMMENT) {
      tokens.push_back(Next());
    }
  }

  tokens.emplace_back(Token{TK_EOF, "", {filePath, lineNumber, characterLineIndex}});

  while(!unclosedDelimiters.empty()) {
    LogError(Errors::UNMATCHED_OP_DELIMITER, unclosedDelimiters.top().GetLocation(), unclosedDelimiters.top().GetStr());
    unclosedDelimiters.pop();
  }

  if (errors.empty()) {
    return tokens;
  } else {
    return errors;
  }
}

template<class... T>
void
Lexer::LogError(Errors::ErrorType errorType, Location location, T&& ... args) {
  if constexpr (sizeof...(args) > 0) {
    errors.push_back(Errors::Generate(errorType, location, inputLines, std::forward<T>(args)...));
  } else {
    errors.push_back(Errors::Generate(errorType, location, inputLines));
  }
}