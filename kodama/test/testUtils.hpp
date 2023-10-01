//
// Created by emma on 29/09/23.
//

#ifndef KODAMA_SRC_TEST_TESTUTILS_HPP_
#define KODAMA_SRC_TEST_TESTUTILS_HPP_

#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../utils/stringUtils.hpp"
#include <gtest/gtest.h>
#include <string>

constexpr std::string DUMMY_FILE_LOCATION = "dummyLocation";

AstNodePtr
buildAST(const std::string& code);

std::vector<Errors::Error>
getErrorsForAst(const std::string& code);

void
AssertEqLocation(const Location& expected, const Location& actual);

#endif //KODAMA_SRC_TEST_TESTUTILS_HPP_