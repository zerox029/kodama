//
// Created by emma on 21/09/23.
//

#include "stringUtils.hpp"

std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter)
{
  std::vector<std::string> strings;

  std::string::size_type pos = 0;
  std::string::size_type prev = 0;
  while ((pos = str.find(delimiter, prev)) != std::string::npos)
  {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + delimiter.size();
  }

  // To get the last substring (or only, if delimiter is not found)
  strings.push_back(str.substr(prev));

  return strings;
}