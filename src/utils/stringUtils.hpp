//
// Created by emma on 21/09/23.
//

#ifndef KODAMA_SRC_UTILS_STRINGUTILS_HPP_
#define KODAMA_SRC_UTILS_STRINGUTILS_HPP_

#include <string>
#include <vector>

const std::string WHITESPACE = " \n\r\t\f\v";

std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);
std::string AddSpace(const std::string& str, size_t s);

std::string LeftTrim(const std::string &s);
std::string RightTrim(const std::string &s);
std::string Trim(const std::string &s);

#endif //KODAMA_SRC_UTILS_STRINGUTILS_HPP_
