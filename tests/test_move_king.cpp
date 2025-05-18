#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare_King) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square("e4");

  EXPECT_THROW(game.generate_king_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotAKing) {
  game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
  const Square pawn_square("d5");

  EXPECT_THROW(game.generate_king_moves(moves, pawn_square), std::runtime_error);
}

TEST_F(MoveTest, CenterKingHasEightMoves) {
  game.from_fen("8/8/8/4K3/8/8/8/8 w - - 0 1");
  const Square king_square("e5");
  const Piece king('K');
  game.generate_king_moves(moves, king_square);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_TRUE(contains_move(king_square, Square("d6"), king));
  EXPECT_TRUE(contains_move(king_square, Square("e6"), king));
  EXPECT_TRUE(contains_move(king_square, Square("f6"), king));
  EXPECT_TRUE(contains_move(king_square, Square("d5"), king));
  EXPECT_TRUE(contains_move(king_square, Square("f5"), king));
  EXPECT_TRUE(contains_move(king_square, Square("d4"), king));
  EXPECT_TRUE(contains_move(king_square, Square("e4"), king));
  EXPECT_TRUE(contains_move(king_square, Square("f4"), king));
}

TEST_F(MoveTest, KingCapturesOpponentPieces) {
  game.from_fen("8/8/8/2PKp3/4p3/8/8/8 w - - 0 1");
  const Square king_square("d5");
  const Piece king('K');
  game.generate_king_moves(moves, king_square);

  EXPECT_TRUE(contains_move(king_square, Square("e4"), king, std::nullopt, Piece('p')));
  EXPECT_TRUE(contains_move(king_square, Square("e5"), king, std::nullopt, Piece('p')));
}

TEST_F(MoveTest, KingCannotMoveIntoOccupiedFriendlySquares) {
  game.from_fen("8/8/8/3K4/2P1N3/8/8/8 w - - 0 1");
  const Square king_square("d5");
  const Piece king('K');
  game.generate_king_moves(moves, king_square);

  EXPECT_FALSE(contains_move(king_square, Square("c4"), king, std::nullopt, Piece('P')));
  EXPECT_FALSE(contains_move(king_square, Square("e4"), king, std::nullopt, Piece('N')));
}

TEST_F(MoveTest, EdgeKingHasFewerMoves) {
  game.from_fen("8/8/8/8/8/8/8/K7 w - - 0 1");
  const Square king_square("a1");
  const Piece king('K');
  game.generate_king_moves(moves, king_square);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(king_square, Square("a2"), king));
  EXPECT_TRUE(contains_move(king_square, Square("b2"), king));
  EXPECT_TRUE(contains_move(king_square, Square("b1"), king));
}
