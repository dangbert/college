#include <iostream>
#include "lab3.h"
#include "/afs/umbc.edu/users/c/m/cmarron/pub//gtest-1.7.0/include/gtest/gtest.h"

using namespace std;

TEST(ChooseTwoTest, HandlesValidInput) {
  EXPECT_EQ(1, ChooseTwo(2));             // lower edge case
  EXPECT_EQ(45, ChooseTwo(10));
  EXPECT_EQ(190, ChooseTwo(20));
  EXPECT_EQ(536854528, ChooseTwo(32768)); // upper edge case
}

TEST(ChooseTwoTest, HandlesInvalidInput) {
  EXPECT_EQ(-1, ChooseTwo(1));            // lower edge case
  EXPECT_EQ(-1, ChooseTwo(0));            
  EXPECT_EQ(-1, ChooseTwo(-1));
  EXPECT_EQ(-1, ChooseTwo(-100));
  EXPECT_EQ(-1, ChooseTwo(32769));        // upper edge case
}

TEST(Factorial, HandlesInvalidInput) {
  EXPECT_EQ(-1, ChooseTwo(1));            // lower edge case                           
  EXPECT_EQ(-1, Factorial(-1));
  EXPECT_EQ(-1, Factorial(-7));
  EXPECT_EQ(-1, Factorial(-100));
  EXPECT_EQ(-1, Factorial(13));        // upper edge case
  EXPECT_EQ(-1, Factorial(100));
}




TEST(FactorialTest, HandlesValidInput) {
  EXPECT_EQ(1, Factorial(0));             // lower edge case
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));

  /* TEST FOR Factorial(12) GOES HERE -- UPPER EDGE CASE */
  EXPECT_EQ(479001600, Factorial(12));

}


/* INVALID INPUT TESTS FOR Factorial() GO HERE */


#define main test_main
#include "lab3.cpp"
#undef main

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

