//
// Created by emma on 30/09/23.
//

#include "../testUtils.hpp"

using Errors::Error;

TEST(ParsingException, HappyPath) {
  try {
    throw ParsingException();
  }
  catch (ParsingException exception){
    EXPECT_EQ(exception.what(), "Aborting due to parsing error...");
  }
}