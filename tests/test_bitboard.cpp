#include <gtest/gtest.h>

#include <chess_engine/bitboard.hpp>
#include <chess_engine/square.hpp>

TEST(BitboardTest, DefaultConstructorInitializesToZero) {
  Bitboard bb;
  EXPECT_EQ(bb.value(), 0ULL);
}

TEST(BitboardTest, ConstructorFromValue) {
  Bitboard bb(0xF0F0F0F0F0F0F0F0ULL);
  EXPECT_EQ(bb.value(), 0xF0F0F0F0F0F0F0F0ULL);
}

TEST(BitboardTest, SetAndTestSquare) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  EXPECT_FALSE(bb.test(Square::E4));
}

TEST(BitboardTest, ClearSquare) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  bb.clear(Square::E2);
  EXPECT_FALSE(bb.test(Square::E2));
}

TEST(BitboardTest, ResetClearsAllSquares) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.reset();
  for (int sq = 0; sq < 64; ++sq) {
    EXPECT_FALSE(bb.test(static_cast<Square>(sq)));
  }
  EXPECT_EQ(bb.value(), 0ULL);
}

TEST(BitboardTest, OrOperatorCombinesBitboards) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::H8);

  Bitboard c = a | b;
  EXPECT_TRUE(c.test(Square::A1));
  EXPECT_TRUE(c.test(Square::H8));
  EXPECT_FALSE(c.test(Square::E4));
}

TEST(BitboardTest, AndOperatorFindsIntersection) {
  Bitboard a, b;
  a.set(Square::E2);
  a.set(Square::E4);
  b.set(Square::E2);
  b.set(Square::D2);

  Bitboard c = a & b;
  EXPECT_TRUE(c.test(Square::E2));
  EXPECT_FALSE(c.test(Square::E4));
  EXPECT_FALSE(c.test(Square::D2));
}

TEST(BitboardTest, CompoundOrAnd) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::B2);

  a |= b;
  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_TRUE(a.test(Square::B2));

  Bitboard c;
  c.set(Square::A1);
  a &= c;
  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_FALSE(a.test(Square::B2));
}

TEST(BitboardTest, NotOperatorInvertsBits) {
  Bitboard a;
  a.set(Square::A1);
  Bitboard b = ~a;
  EXPECT_FALSE(b.test(Square::A1));
  EXPECT_TRUE(b.test(Square::B1));  // something that wasn’t set
}

TEST(BitboardTest, EqualityOperators) {
  Bitboard a, b;
  a.set(Square::E2);
  b.set(Square::E2);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  b.set(Square::E4);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}