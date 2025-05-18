#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square("e5");

  EXPECT_THROW(game.generate_rook_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotARook) {
  game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
  const Square pawn_square("d5");

  EXPECT_THROW(game.generate_rook_moves(moves, pawn_square), std::runtime_error);
}

TEST_F(MoveTest, CenterRookHasFourteenMoves) {
  game.from_fen("8/8/8/3R4/8/8/8/8 w - - 0 1");
  const Square rook_square("d5");
  const Piece rook('R');
  game.generate_rook_moves(moves, rook_square);

  EXPECT_EQ(moves.size(), 14);
  // Vertical moves
  EXPECT_TRUE(contains_move(rook_square, Square("d6"), rook));  // d5d6
  EXPECT_TRUE(contains_move(rook_square, Square("d7"), rook));  // d5d7
  EXPECT_TRUE(contains_move(rook_square, Square("d8"), rook));  // d5d8
  EXPECT_TRUE(contains_move(rook_square, Square("d4"), rook));  // d5d4
  EXPECT_TRUE(contains_move(rook_square, Square("d3"), rook));  // d5d3
  EXPECT_TRUE(contains_move(rook_square, Square("d2"), rook));  // d5d2
  EXPECT_TRUE(contains_move(rook_square, Square("d1"), rook));  // d5d1
  // Horizontal moves
  EXPECT_TRUE(contains_move(rook_square, Square("a5"), rook));  // d5a5
  EXPECT_TRUE(contains_move(rook_square, Square("b5"), rook));  // d5b5
  EXPECT_TRUE(contains_move(rook_square, Square("c5"), rook));  // d5c5
  EXPECT_TRUE(contains_move(rook_square, Square("e5"), rook));  // d5e5
  EXPECT_TRUE(contains_move(rook_square, Square("f5"), rook));  // d5f5
  EXPECT_TRUE(contains_move(rook_square, Square("g5"), rook));  // d5g5
  EXPECT_TRUE(contains_move(rook_square, Square("h5"), rook));  // d5h5
}

TEST_F(MoveTest, RookCapturesOpponentPieces) {
  game.from_fen("8/8/8/3Rr3/8/3p4/8/8 w - - 0 1");
  const Square rook_square("d5");
  const Piece rook('R');
  game.generate_rook_moves(moves, rook_square);

  EXPECT_EQ(moves.size(), 9);
  EXPECT_TRUE(contains_move(rook_square, Square("e5"), rook, std::nullopt, Piece('r')));  // d5xe5
  EXPECT_TRUE(contains_move(rook_square, Square("d3"), rook, std::nullopt, Piece('p')));  // d5xd3
}

TEST_F(MoveTest, RookCannotMoveThroughPieces) {
  game.from_fen("8/8/8/3Rr3/3P4/8/8/8 w - - 0 1");
  const Square rook_square("d5");
  const Piece rook('R');
  game.generate_rook_moves(moves, rook_square);

  // Should not include the square behind the blocking piece
  EXPECT_EQ(moves.size(), 7);
  EXPECT_FALSE(contains_move(rook_square, Square("f5"), rook));  // blocked beyond r
  EXPECT_FALSE(contains_move(rook_square, Square("g5"), rook));  // blocked beyond r
  EXPECT_FALSE(contains_move(rook_square, Square("h5"), rook));  // blocked beyond r
  EXPECT_FALSE(contains_move(rook_square, Square("d4"), rook));  // blocked by P
  EXPECT_FALSE(contains_move(rook_square, Square("d3"), rook));  // blocked beyond P
  EXPECT_FALSE(contains_move(rook_square, Square("d2"), rook));  // blocked beyond P
  EXPECT_FALSE(contains_move(rook_square, Square("d1"), rook));  // blocked beyond P
}

TEST_F(MoveTest, RookCannotCaptureFriendlyPieces) {
  game.from_fen("8/8/8/3R4/3P4/3N4/8/8 w - - 0 1");
  const Square rook_square("d5");
  const Piece rook('R');
  game.generate_rook_moves(moves, rook_square);

  EXPECT_FALSE(contains_move(rook_square, Square("d4"), rook, std::nullopt, Piece('P')));  // d5xd4
  EXPECT_FALSE(contains_move(rook_square, Square("d3"), rook, std::nullopt, Piece('N')));  // d5xd3
}
