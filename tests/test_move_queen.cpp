#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare_Queen) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square("d4");

  EXPECT_THROW(game.generate_queen_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotAQueen) {
  game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
  const Square pawn_square("d5");

  EXPECT_THROW(game.generate_queen_moves(moves, pawn_square), std::runtime_error);
}

TEST_F(MoveTest, CenterQueenHasTwentySevenMoves) {
  game.from_fen("8/8/8/3Q4/8/8/8/8 w - - 0 1");
  const Square queen_square("d5");
  const Piece queen('Q');
  game.generate_queen_moves(moves, queen_square);

  EXPECT_EQ(moves.size(), 27);
  // Diagonal moves
  EXPECT_TRUE(contains_move(queen_square, Square("e6"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("f7"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("g8"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("e4"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("f3"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("g2"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("h1"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("c6"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("b7"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("a8"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("c4"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("b3"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("a2"), queen));

  // Horizontal and vertical moves
  EXPECT_TRUE(contains_move(queen_square, Square("d6"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d7"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d8"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d4"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d3"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d2"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("d1"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("e5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("f5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("g5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("h5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("c5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("b5"), queen));
  EXPECT_TRUE(contains_move(queen_square, Square("a5"), queen));
}

TEST_F(MoveTest, QueenCapturesOpponentPieces) {
  game.from_fen("8/8/2p5/3Qp3/2P5/8/8/8 w - - 0 1");
  const Square queen_square("d5");
  const Piece queen('Q');
  game.generate_queen_moves(moves, queen_square);

  EXPECT_TRUE(contains_move(queen_square, Square("c6"), queen, std::nullopt, Piece('p')));
  EXPECT_TRUE(contains_move(queen_square, Square("e5"), queen, std::nullopt, Piece('p')));
}

TEST_F(MoveTest, QueenCannotMoveThroughPieces) {
  game.from_fen("8/8/2p5/3Qr3/2P5/8/8/8 w - - 0 1");
  const Square queen_square("d5");
  const Piece queen('Q');
  game.generate_queen_moves(moves, queen_square);

  // Blocked diagonal
  EXPECT_FALSE(contains_move(queen_square, Square("a2"), queen));
  EXPECT_FALSE(contains_move(queen_square, Square("b3"), queen));
  EXPECT_FALSE(contains_move(queen_square, Square("a8"), queen));
  EXPECT_FALSE(contains_move(queen_square, Square("b7"), queen));

  // Blocked horizontal
  EXPECT_FALSE(contains_move(queen_square, Square("f5"), queen));
  EXPECT_FALSE(contains_move(queen_square, Square("g5"), queen));
  EXPECT_FALSE(contains_move(queen_square, Square("h5"), queen));
}

TEST_F(MoveTest, QueenCannotCaptureFriendlyPieces) {
  game.from_fen("8/8/8/3P4/2Q5/1N6/8/8 w - - 0 1");
  const Square queen_square("c4");
  const Piece queen('Q');
  game.generate_queen_moves(moves, queen_square);

  EXPECT_FALSE(contains_move(queen_square, Square("b3"), queen, std::nullopt, Piece('N')));
  EXPECT_FALSE(contains_move(queen_square, Square("d5"), queen, std::nullopt, Piece('P')));
}
