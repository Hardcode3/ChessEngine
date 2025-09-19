#include <gtest/gtest.h>

#include <chess_engine/square.hpp>

// --- Construction Tests ---
TEST(SquareTest, ConstructFromFileRankValid) {
  Square sq(4, 3);  // e4
  EXPECT_EQ(sq.file(), 4);
  EXPECT_EQ(sq.rank(), 3);
  EXPECT_EQ(sq.to_string(), "e4");
}

TEST(SquareTest, ConstructFromFileRankInvalid) {
  EXPECT_THROW(Square(8, 0), std::invalid_argument);
  EXPECT_THROW(Square(0, -1), std::invalid_argument);
}

TEST(SquareTest, ConstructFromStringValid) {
  Square sq("e4");
  EXPECT_EQ(sq.file(), 4);
  EXPECT_EQ(sq.rank(), 3);
  EXPECT_EQ(sq.to_string(), "e4");
}

TEST(SquareTest, ConstructFromStringInvalid) {
  EXPECT_THROW(Square("z9"), std::invalid_argument);
  EXPECT_THROW(Square("abc"), std::invalid_argument);
  EXPECT_THROW(Square(""), std::invalid_argument);
}

// --- Equality ---
TEST(SquareTest, EqualityOperators) {
  Square s1("e4");
  Square s2(4, 3);
  Square s3("d5");

  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 == s3);
  EXPECT_TRUE(s1 != s3);
}

// --- Enum Mapping ---
TEST(SquareTest, EnumValueMapping) {
  Square s1(Square::A1);
  Square s2(Square::H8);

  EXPECT_EQ(s1.to_string(), "a1");
  EXPECT_EQ(s2.to_string(), "h8");
  EXPECT_EQ(s1.file(), 0);
  EXPECT_EQ(s1.rank(), 0);
  EXPECT_EQ(s2.file(), 7);
  EXPECT_EQ(s2.rank(), 7);
}
