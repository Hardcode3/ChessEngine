#include <gtest/gtest.h>

#include <chess_engine/bitboard.hpp>
#include <chess_engine/square.hpp>
#include <sstream>

/**
 * @test Print function.
 * @brief Verifies that the bitboard prints as an 8×8 grid with correct layout.
 */
TEST(BitboardTest, PrintOutputsCorrectBoard) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.set(Square::E4);

  /* Expects:
  . . . . . . . 1 \n    H8
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . 1 . . . \n    E4
  . . . . . . . . \n
  . . . . . . . . \n
  1 . . . . . . . \n" A1
  */

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  bb.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);

  std::string output = oss.str();

  // Check key positions:
  // - H8 (top right corner) should be "1"
  EXPECT_NE(output.find(". . . . . . . 1"), std::string::npos);

  // - E4 should be in the middle
  EXPECT_NE(output.find(". . . . 1 . . ."), std::string::npos);

  // - A1 (bottom left corner) should be "1" in the last row
  EXPECT_NE(output.rfind("1 . . . . . . ."), std::string::npos);

  // Sanity check: must contain 8 lines (ranks 8 → 1)
  int line_count = 0;
  for (char c : output) {
    if (c == '\n') ++line_count;
  }
  EXPECT_EQ(line_count, 8);
}

/**
 * @test Verify default constructor.
 * @brief Ensures a new Bitboard starts empty (all bits = 0).
 */
TEST(BitboardTest, DefaultConstructorInitializesToZero) {
  Bitboard bb;
  EXPECT_EQ(bb.value(), 0ULL);
}

/**
 * @test Construct from a raw 64-bit value.
 * @brief Confirms the stored value matches the given constant.
 */
TEST(BitboardTest, ConstructorFromValue) {
  Bitboard bb(0xF0F0F0F0F0F0F0F0ULL);
  EXPECT_EQ(bb.value(), 0xF0F0F0F0F0F0F0F0ULL);
}

/**
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  EXPECT_FALSE(bb.test(Square::E4));
}

/**
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);
  Square e4(Square::E4);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  EXPECT_FALSE(bb.test(e4));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  bb.clear(Square::E2);
  EXPECT_FALSE(bb.test(Square::E2));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  bb.clear(e2);
  EXPECT_FALSE(bb.test(e2));
}

/**
 * @test Reset the bitboard.
 * @brief Ensures all bits are cleared and value() = 0.
 */
TEST(BitboardTest, ResetClearsAllSquares) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.reset();
  for (int sq = 0; sq < 64; ++sq) {
    Square square(sq);
    EXPECT_FALSE(bb.test(square));
  }
  EXPECT_EQ(bb.value(), 0ULL);
}

/**
 * @test Bitwise OR operator.
 * @brief Combines two bitboards, keeping all occupied squares.
 */
TEST(BitboardTest, OrOperatorCombinesBitboards) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::H8);

  Bitboard c = a | b;
  EXPECT_TRUE(c.test(Square::A1));
  EXPECT_TRUE(c.test(Square::H8));
  EXPECT_FALSE(c.test(Square::E4));
}

/**
 * @test Bitwise AND operator.
 * @brief Returns the intersection of two bitboards (only common bits set).
 */
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

/**
 * @test Compound OR and AND operators.
 * @brief Ensures |= adds squares and &= keeps only intersections.
 */
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

/**
 * @test Bitwise NOT operator.
 * @brief Inverts all bits: occupied squares become empty and vice versa.
 */
TEST(BitboardTest, NotOperatorInvertsBits) {
  Bitboard a;
  a.set(Square::A1);
  Bitboard b = ~a;
  EXPECT_FALSE(b.test(Square::A1));
  EXPECT_TRUE(b.test(Square::B1));  // square that wasn’t set
}

/**
 * @test Equality and inequality operators.
 * @brief Confirms bitboards compare equal only when they have identical bits.
 */
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

/**
 * @test Left bitshift operator.
 * @brief Confirms left bitshift operator works as intended.
 */
TEST(BitboardTest, ShiftLeft) {
  Bitboard b1(0x1);                      // binary: ...0001  (only bit 0 set -> square A1)
  EXPECT_EQ((b1 << 1).value(), 0x2ULL);  // binary: ...0010  (only bit 1 set -> square B1)
  EXPECT_EQ((b1 << 3).value(), 0x8ULL);  // binary: ...1000  (only bit 3 set -> square D1)

  Bitboard b2(0x100);                       // binary: ...0000 0001 0000 0000  (only bit 8 set -> square A2)
  EXPECT_EQ((b2 << 4).value(), 0x1000ULL);  // binary: ...0001 0000 0000 0000 (only bit 12 set -> square E2)
}

/**
 * @test Right bitshift operator.
 * @brief Confirms right bitshift operator works as intended.
 */
TEST(BitboardTest, ShiftRight) {
  Bitboard b1(0x8);                      // binary: ...1000  (only bit 3 set -> square D1)
  EXPECT_EQ((b1 >> 3).value(), 0x1ULL);  // binary: ...0001  (only bit 0 set -> square A1)

  Bitboard b2(0x1000);                     // binary: ...0001 0000 0000 0000  (only bit 12 set -> square E2)
  EXPECT_EQ((b2 >> 4).value(), 0x100ULL);  // binary: ...0000 0001 0000 0000  (only bit 8 set -> square A2)
}
