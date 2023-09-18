//
// Created by emma on 17/09/23.
//

#include <gtest/gtest.h>

class myTestFixture: public ::testing::Test {
 public:
  myTestFixture( ) {
// initialization;
// can also be done in SetUp()
  }

  void SetUp( ) {
// initialization or some code to run before each test
  }

  void TearDown( ) {
// code to run after each test;
// can be used instead of a destructor,
// but exceptions can be handled in this function only
  }

  ~myTestFixture( )  {
//resources cleanup, no exceptions allowed
  }

// shared user data
};

TEST (SquareTest /*test suite name*/, PosZeroNeg /*test name*/) {
  EXPECT_EQ (9.0, (3.0*2.0)); // fail, test continues
  ASSERT_EQ (0.0, (0.0));     // success
  ASSERT_EQ (9, (3)*(-3.0));  // fail, test interrupts
  ASSERT_EQ (-9, (-3)*(-3.0));// not executed due to the previous assert
}