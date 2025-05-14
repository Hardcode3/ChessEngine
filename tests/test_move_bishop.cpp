#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare_Bishop) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square("e5");

  EXPECT_THROW(game.generate_bishop_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotABishop) {
  game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
  const Square pawn_square("d5");

  EXPECT_THROW(game.generate_bishop_moves(moves, pawn_square), std::runtime_error);
}

TEST_F(MoveTest, CenterBishopHasThirteenMoves) {
  game.from_fen("8/8/8/3B4/8/8/8/8 w - - 0 1");
  const Square bishop_square("d5");
  const Piece bishop('B');
  game.generate_bishop_moves(moves, bishop_square);

  EXPECT_EQ(moves.size(), 13);
  // Diagonal moves
  EXPECT_TRUE(contains_move(bishop_square, Square("e6"), bishop));  // d5e6
  EXPECT_TRUE(contains_move(bishop_square, Square("f7"), bishop));  // d5f7
  EXPECT_TRUE(contains_move(bishop_square, Square("g8"), bishop));  // d5g8
  EXPECT_TRUE(contains_move(bishop_square, Square("e4"), bishop));  // d5e4
  EXPECT_TRUE(contains_move(bishop_square, Square("f3"), bishop));  // d5f3
  EXPECT_TRUE(contains_move(bishop_square, Square("g2"), bishop));  // d5g2
  EXPECT_TRUE(contains_move(bishop_square, Square("h1"), bishop));  // d5h1
  EXPECT_TRUE(contains_move(bishop_square, Square("c6"), bishop));  // d5c6
  EXPECT_TRUE(contains_move(bishop_square, Square("b7"), bishop));  // d5b7
  EXPECT_TRUE(contains_move(bishop_square, Square("a8"), bishop));  // d5a8
  EXPECT_TRUE(contains_move(bishop_square, Square("c4"), bishop));  // d5c4
  EXPECT_TRUE(contains_move(bishop_square, Square("b3"), bishop));  // d5b3
  EXPECT_TRUE(contains_move(bishop_square, Square("a2"), bishop));  // d5a2
}

TEST_F(MoveTest, BishopCapturesOpponentPieces) {
  game.from_fen("8/8/2p5/3Br3/2P5/8/8/8 w - - 0 1");
  const Square bishop_square("d5");
  const Piece bishop('B');
  game.generate_bishop_moves(moves, bishop_square);

  EXPECT_TRUE(contains_move(bishop_square, Square("c6"), bishop, std::nullopt, Piece('p')));  // d5xc6
}

TEST_F(MoveTest, BishopCannotMoveThroughPieces) {
  game.from_fen("8/8/2p5/3Br3/2P5/8/8/8 w - - 0 1");
  const Square bishop_square("d5");
  const Piece bishop('B');
  game.generate_bishop_moves(moves, bishop_square);

  EXPECT_FALSE(contains_move(bishop_square, Square("a2"), bishop));  // blocked beyond P
  EXPECT_FALSE(contains_move(bishop_square, Square("b3"), bishop));  // blocked beyond P
  EXPECT_FALSE(contains_move(bishop_square, Square("a8"), bishop));  // blocked beyond p
  EXPECT_FALSE(contains_move(bishop_square, Square("b7"), bishop));  // blocked beyond p
}

TEST_F(MoveTest, BishopCannotCaptureFriendlyPieces) {
  game.from_fen("8/8/8/3P4/2B5/1N6/8/8 w - - 0 1");
  const Square bishop_square("c4");
  const Piece bishop('B');
  game.generate_bishop_moves(moves, bishop_square);

  EXPECT_FALSE(contains_move(bishop_square, Square("b3"), bishop, std::nullopt, Piece('N')));  // c4xb3
  EXPECT_FALSE(contains_move(bishop_square, Square("d5"), bishop, std::nullopt, Piece('P')));  // c4xd5
}