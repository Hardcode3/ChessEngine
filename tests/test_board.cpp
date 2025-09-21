#include <gtest/gtest.h>

#include <chess_engine/board.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <sstream>

/**
 * @test BoardTest.DefaultStartingPosDefaultConstructor
 * @brief Verifies that a newly constructed board has the default FEN starting postion
 */
TEST(BoardTest, EmptyBoardInitially) {
  Board board;

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('R'));
  EXPECT_EQ(board.get_piece(Square(4, 0)), Piece('K'));
  EXPECT_EQ(board.get_piece(Square(0, 7)), Piece('r'));
  EXPECT_EQ(board.get_piece(Square(4, 7)), Piece('k'));

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Piece('P'));
    EXPECT_EQ(board.get_piece(Square(file, 6)), Piece('p'));
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test BoardTest.FENConstructor
 * @brief Verifies constructing a board from FEN string.
 *
 * Example FEN: standard starting position.
 */
TEST(BoardTest, FENConstructor) {
  std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Board board(start_fen);

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('R'));
  EXPECT_EQ(board.get_piece(Square(4, 0)), Piece('K'));
  EXPECT_EQ(board.get_piece(Square(0, 7)), Piece('r'));
  EXPECT_EQ(board.get_piece(Square(4, 7)), Piece('k'));

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Piece('P'));
    EXPECT_EQ(board.get_piece(Square(file, 6)), Piece('p'));
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test BoardTest.SetAndGetPiece
 * @brief Verifies placing and retrieving pieces.
 *
 * Checks:
 * - Placing white rook, white king, black queen
 * - Pieces correctly returned via get_piece
 * - Piece presence reflected in color-specific bitboards
 * - Occupied bitboard updated
 */
TEST(BoardTest, SetAndGetPiece) {
  Board board;

  board.set_piece(Square(0, 0), Piece('R'));  // White rook
  board.set_piece(Square(4, 0), Piece('K'));  // White king
  board.set_piece(Square(3, 7), Piece('q'));  // Black queen

  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('R'));
  EXPECT_EQ(board.get_piece(Square(4, 0)), Piece('K'));
  EXPECT_EQ(board.get_piece(Square(3, 7)), Piece('q'));

  EXPECT_TRUE(board.white_pieces().test(Square(0, 0)));
  EXPECT_TRUE(board.white_pieces().test(Square(4, 0)));
  EXPECT_TRUE(board.black_pieces().test(Square(3, 7)));

  EXPECT_TRUE(board.occupied().test(Square(0, 0)));
  EXPECT_TRUE(board.occupied().test(Square(4, 0)));
  EXPECT_TRUE(board.occupied().test(Square(3, 7)));
}

/**
 * @test BoardTest.RemovePiece
 * @brief Verifies removing a piece from a square.
 */
TEST(BoardTest, RemovePiece) {
  Board board;
  board.set_piece(Square(0, 0), Piece('R'));
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('R'));

  board.remove_piece(Square(0, 0));
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('.'));
  EXPECT_FALSE(board.occupied().test(Square(0, 0)));
}

/**
 * @test BoardTest.ReplacePiece
 * @brief Verifies replacing a piece with another of a different color.
 *
 * Checks:
 * - Original piece replaced
 * - Bitboards updated correctly (no ghost bits left)
 */
TEST(BoardTest, ReplacePiece) {
  Board board;
  board.set_piece(Square(0, 0), Piece('R'));
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('R'));

  board.set_piece(Square(0, 0), Piece('n'));  // replace with black knight
  EXPECT_EQ(board.get_piece(Square(0, 0)), Piece('n'));

  EXPECT_TRUE(board.black_pieces().test(Square(0, 0)));
  EXPECT_FALSE(board.white_pieces().test(Square(0, 0)));
}

/**
 * @test BoardTest.PrintBoard
 * @brief Verifies that the board prints in correct ASCII format.
 */
TEST(BoardTest, PrintBoard) {
  Board board;

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  board.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);
  std::string output = oss.str();

  std::string expected =
      "8 r n b q k b n r \n"
      "7 p p p p p p p p \n"
      "6 . . . . . . . . \n"
      "5 . . . . . . . . \n"
      "4 . . . . . . . . \n"
      "3 . . . . . . . . \n"
      "2 P P P P P P P P \n"
      "1 R N B Q K B N R \n"
      "  a b c d e f g h\n";

  EXPECT_EQ(output, expected);
}
