#include <gtest/gtest.h>

#include <chess_engine/board.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <sstream>

// Helper: construct a Square from file (0..7) and rank (0..7)
static Square sq(int file, int rank) { return Square(rank * 8 + file); }

TEST(BoardTest, EmptyBoardInitially) {
  Board board;

  EXPECT_EQ(board.white_pieces().value(), 0ULL);
  EXPECT_EQ(board.black_pieces().value(), 0ULL);
  EXPECT_EQ(board.occupied().value(), 0ULL);

  for (int i = 0; i < 64; i++) {
    EXPECT_EQ(board.get_piece(Square(i)), Piece('.'));
  }
}

TEST(BoardTest, SetAndGetPiece) {
  Board board;

  board.set_piece(sq(0, 0), Piece('R'));  // White rook
  board.set_piece(sq(4, 0), Piece('K'));  // White king
  board.set_piece(sq(3, 7), Piece('q'));  // Black queen

  EXPECT_EQ(board.get_piece(sq(0, 0)), Piece('R'));
  EXPECT_EQ(board.get_piece(sq(4, 0)), Piece('K'));
  EXPECT_EQ(board.get_piece(sq(3, 7)), Piece('q'));

  EXPECT_TRUE(board.white_pieces().test(sq(0, 0)));
  EXPECT_TRUE(board.white_pieces().test(sq(4, 0)));
  EXPECT_TRUE(board.black_pieces().test(sq(3, 7)));

  EXPECT_TRUE(board.occupied().test(sq(0, 0)));
  EXPECT_TRUE(board.occupied().test(sq(4, 0)));
  EXPECT_TRUE(board.occupied().test(sq(3, 7)));
}

TEST(BoardTest, RemovePiece) {
  Board board;
  board.set_piece(sq(0, 0), Piece('R'));
  EXPECT_EQ(board.get_piece(sq(0, 0)), Piece('R'));

  board.remove_piece(sq(0, 0));
  EXPECT_EQ(board.get_piece(sq(0, 0)), Piece('.'));
  EXPECT_FALSE(board.occupied().test(sq(0, 0)));
}

TEST(BoardTest, ReplacePiece) {
  Board board;
  board.set_piece(sq(0, 0), Piece('R'));
  EXPECT_EQ(board.get_piece(sq(0, 0)), Piece('R'));

  board.set_piece(sq(0, 0), Piece('n'));  // replace with black knight
  EXPECT_EQ(board.get_piece(sq(0, 0)), Piece('n'));

  EXPECT_TRUE(board.black_pieces().test(sq(0, 0)));
  EXPECT_FALSE(board.white_pieces().test(sq(0, 0)));
}

TEST(BoardTest, PrintBoard) {
  Board board;
  board.set_piece(sq(0, 0), Piece('R'));
  board.set_piece(sq(4, 0), Piece('K'));
  board.set_piece(sq(7, 7), Piece('k'));

  std::ostringstream oss;
  board.print(oss);

  std::string expected =
      "8 . . . . . . . k \n"
      "7 . . . . . . . . \n"
      "6 . . . . . . . . \n"
      "5 . . . . . . . . \n"
      "4 . . . . . . . . \n"
      "3 . . . . . . . . \n"
      "2 . . . . . . . . \n"
      "1 R . . . K . . . \n"
      "  a b c d e f g h\n";

  EXPECT_EQ(oss.str(), expected);
}