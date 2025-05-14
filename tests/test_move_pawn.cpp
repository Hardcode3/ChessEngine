#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, ThrowsIfNoPieceAtSquare) {
  game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
  const Square empty_square(4, 4);  // e5

  EXPECT_THROW(game.generate_pawn_moves(moves, empty_square), std::runtime_error);
}

TEST_F(MoveTest, ThrowsIfNotAKnight) {
  game.from_fen("8/8/8/3N4/8/8/8/8 w - - 0 1");
  const Square knight_square(3, 4);  // d5

  EXPECT_THROW(game.generate_pawn_moves(moves, knight_square), std::runtime_error);
}

TEST_F(MoveTest, WhitePawnStartingPosition) {
  // Set up a white pawn at its starting position
  game.from_fen("8/8/8/8/8/8/P7/8 w - - 0 1");
  const Square pawn_square(0, 1);  // a2
  game.generate_pawn_moves(moves, pawn_square);
  const Piece pawn('P');

  // Should have 2 moves: one square forward and two squares forward
  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 2), pawn));  // a2-a3
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 3), pawn));  // a2-a4
}

TEST_F(MoveTest, BlackPawnStartingPosition) {
  // Set up a black pawn at its starting position
  game.from_fen("8/p7/8/8/8/8/8/8 b - - 0 1");
  const Square pawn_square(0, 6);  // a7
  game.generate_pawn_moves(moves, pawn_square);
  const Piece pawn('p');

  // Should have 2 moves: one square forward and two squares forward
  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 5), pawn));  // a7-a6
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 4), pawn));  // a7-a5
}

TEST_F(MoveTest, WhitePawnPromotion) {
  // Set up a white pawn ready to promote
  game.from_fen("8/P7/8/8/8/8/8/8 w - - 0 1");
  const Square from(0, 6);  // a7
  const Square to(0, 7);    // a8
  const Piece pawn('P');
  game.generate_pawn_moves(moves, from);

  // Should have 4 promotion moves (one for each piece type)
  EXPECT_EQ(moves.size(), 4);
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('Q')));  // a7-a8=Q
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('R')));  // a7-a8=R
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('B')));  // a7-a8=B
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('N')));  // a7-a8=N
}

TEST_F(MoveTest, BlackPawnPromotion) {
  // Set up a black pawn ready to promote
  game.from_fen("8/8/8/8/8/8/7p/8 b - - 0 1");
  const Square from(7, 1);  // h2
  const Square to(7, 0);    // h1
  const Piece pawn('p');
  game.generate_pawn_moves(moves, from);

  // Should have 4 promotion moves (one for each piece type)
  EXPECT_EQ(moves.size(), 4);
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('q')));  // h2-h1=q
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('r')));  // h2-h1=r
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('b')));  // h2-h1=b
  EXPECT_TRUE(contains_move(from, to, pawn, Piece('n')));  // h2-h1=n
}

TEST_F(MoveTest, WhitePawnCapture) {
  // Set up a white pawn with capture opportunities
  game.from_fen("8/8/8/8/8/1p6/P7/8 w - - 0 1");
  const Square pawn_square(0, 1);  // a2
  const Piece white_pawn('P');
  game.generate_pawn_moves(moves, pawn_square);

  // Should have 3 moves: one forward, two forward, and one capture
  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 2), white_pawn));                            // a2-a3
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 3), white_pawn));                            // a2-a4
  EXPECT_TRUE(contains_move(pawn_square, Square(1, 2), white_pawn, std::nullopt, Piece('p')));  // a2xb3
}

TEST_F(MoveTest, BlackPawnCapture) {
  // Set up a black pawn with capture opportunities
  game.from_fen("8/p7/1P6/8/8/8/8/8 b - - 0 1");
  const Square pawn_square(0, 6);  // a7
  game.generate_pawn_moves(moves, pawn_square);

  // Should have 3 moves: one forward, two forward, and one capture
  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 5), Piece('p')));                            // a7-a6
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 4), Piece('p')));                            // a7-a5
  EXPECT_TRUE(contains_move(pawn_square, Square(1, 5), Piece('p'), std::nullopt, Piece('P')));  // a7xb6
}

TEST_F(MoveTest, BlackPawnEnPassant) {
  // Set up a position where en passant is possible
  game.from_fen("8/8/8/8/pP6/8/8/8 b - b3 0 1");
  const Square pawn_square(0, 3);  // a4
  const Piece black_pawn('p');
  game.generate_pawn_moves(moves, pawn_square);

  // Should have 2 moves: one forward and one en passant capture
  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(pawn_square, Square(0, 2), black_pawn));                            // a4-a3
  EXPECT_TRUE(contains_move(pawn_square, Square(1, 2), black_pawn, std::nullopt, Piece('P')));  // a4xb3 (en passant)
}

TEST_F(MoveTest, WhitePawnEnPassant) {
  // Set up a position where en passant is possible
  game.from_fen("8/8/8/5pP1/8/8/8/8 w - f6 0 1");
  const Square pawn_square(6, 4);  // g5
  const Piece white_pawn('P');
  game.generate_pawn_moves(moves, pawn_square);

  // Should have 2 moves: one forward and one en passant capture
  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(pawn_square, Square(6, 5), white_pawn));                            // g5-g6
  EXPECT_TRUE(contains_move(pawn_square, Square(5, 5), white_pawn, std::nullopt, Piece('p')));  // g5xf6 (en passant)
}

TEST_F(MoveTest, WhitePawnIsBlocked) {
  // Set up a position where a pawn is blocked
  game.from_fen("8/8/8/8/8/1p6/1P6/8 w - - 0 1");
  const Square pawn_square(1, 1);  // b2
  game.generate_pawn_moves(moves, pawn_square);

  // Should have no moves as the pawn is blocked
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(MoveTest, BlackPawnIsBlocked) {
  // Set up a position where a pawn is blocked
  game.from_fen("8/8/8/8/1p6/1P6/8/8 b - - 0 1");
  const Square pawn_square(1, 3);  // b4
  game.generate_pawn_moves(moves, pawn_square);

  // Should have no moves as the pawn is blocked
  EXPECT_EQ(moves.size(), 0);
}
