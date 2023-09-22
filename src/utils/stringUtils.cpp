//
// Created by emma on 21/09/23.
//

#include "stringUtils.hpp"

std::vector<std::string>
SplitString(const std::string& str, const std::string& delimiter) {
  std::vector<std::string> strings;

  std::string::size_type pos = 0;
  std::string::size_type prev = 0;
  while ((pos = str.find(delimiter, prev)) != std::string::npos) {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + delimiter.size();
  }

  // To get the last substring (or only, if delimiter is not found)
  strings.push_back(str.substr(prev));

  return strings;
}

std::string
AddSpace(const std::string& str, size_t s) {
  std::string res;
  if (s == 0) return str;
  if (str.size() > (s + 1)) {
    for (auto& c : str) {
      if (c == '\t') {
        res += '\t';
      } else {
        res += ' ';
      }
    }
  } else {
    for (size_t i = 0; i < (s - 1); ++i) {
      if (str[i] == '\t') {
        res += '\t';
      } else {
        res += ' ';
      }
    }
  }
  return res;
};