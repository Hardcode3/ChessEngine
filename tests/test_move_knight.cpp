#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>
#include <iostream>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square(4, 4);  // e5

  EXPECT_THROW(game.generate_knight_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotAKnight) {
  game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
  const Square pawn_square(3, 4);  // d5

  EXPECT_THROW(game.generate_knight_moves(moves, pawn_square), std::runtime_error);
}

TEST_F(MoveTest, CenterKnightHasEightMoves) {
  game.from_fen("8/8/8/3N4/8/8/8/8 w - - 0 1");
  const Square knight_square(3, 4);  // d5
  const Piece knight('N');
  game.generate_knight_moves(moves, knight_square);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_TRUE(contains_move(knight_square, Square(4, 6), knight));  // d5e7
  EXPECT_TRUE(contains_move(knight_square, Square(2, 6), knight));  // d5c7
  EXPECT_TRUE(contains_move(knight_square, Square(1, 5), knight));  // d5b6
  EXPECT_TRUE(contains_move(knight_square, Square(1, 3), knight));  // d5b4
  EXPECT_TRUE(contains_move(knight_square, Square(2, 2), knight));  // d5c3
  EXPECT_TRUE(contains_move(knight_square, Square(4, 2), knight));  // d5e3
  EXPECT_TRUE(contains_move(knight_square, Square(5, 3), knight));  // d5f4
  EXPECT_TRUE(contains_move(knight_square, Square(5, 5), knight));  // d5f6
}

TEST_F(MoveTest, CornerKnightHasTwoMoves) {
  game.from_fen("N7/8/8/8/8/8/8/8 w - - 0 1");
  const Square knight_square(0, 7);  // a8
  const Piece knight('N');
  game.generate_knight_moves(moves, knight_square);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(knight_square, Square(1, 5), knight));  // a8b6
  EXPECT_TRUE(contains_move(knight_square, Square(2, 6), knight));  // a8c7
}

TEST_F(MoveTest, KnightCapturesOpponentPieces) {
  game.from_fen("8/8/1p6/3N4/1r6/8/8/8 w - - 0 1");
  const Square knight_square(3, 4);  // d5
  const Piece knight('N');
  game.generate_knight_moves(moves, knight_square);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_TRUE(contains_move(knight_square, Square(1, 5), knight, std::nullopt, Piece('p')));  // d5xb6
  EXPECT_TRUE(contains_move(knight_square, Square(1, 3), knight, std::nullopt, Piece('r')));  // d5xb4
}

TEST_F(MoveTest, KnightCannotCaptureFriendlyPieces) {
  game.from_fen("8/8/1P6/3N4/1R6/8/8/8 w - - 0 1");
  const Square knight_square(3, 4);  // d5
  const Piece knight('N');
  game.generate_knight_moves(moves, knight_square);

  EXPECT_EQ(moves.size(), 6);
  EXPECT_TRUE(!contains_move(knight_square, Square(1, 5), knight, std::nullopt, Piece('P')));  // d5xb6
  EXPECT_TRUE(!contains_move(knight_square, Square(1, 3), knight, std::nullopt, Piece('R')));  // d5xb4
}
