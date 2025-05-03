#include <gtest/gtest.h>
#include <chess_engine/game.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <chess_engine/move.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

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

// TEST_F(KnightMoveTest, CornerKnightHasTwoMoves) {
//     game.from_fen("N7/8/8/8/8/8/8/8 w - - 0 1");
//     const Square knight_square(0, 7); // a8
//     const Piece knight('N');
//     game.generate_knight_moves(moves, knight_square);

//     EXPECT_EQ(moves.size(), 2);
//     EXPECT_TRUE(contains_move(knight_square, Square(1, 5), knight));
//     EXPECT_TRUE(contains_move(knight_square, Square(2, 6), knight));
// }

// TEST_F(KnightMoveTest, KnightCapturesOpponentPieces) {
//     game.from_fen("8/8/2p5/3N4/2p5/8/8/8 w - - 0 1");
//     const Square knight_square(3, 4); // d5
//     const Piece knight('N');
//     game.generate_knight_moves(moves, knight_square);

//     // Should capture pawns on c6 and c4
//     EXPECT_TRUE(contains_move(knight_square, Square(2, 6), knight, Piece('p')));
//     EXPECT_TRUE(contains_move(knight_square, Square(2, 2), knight, Piece('p')));
// }

// TEST_F(KnightMoveTest, KnightCannotCaptureFriendlyPieces) {
//     game.from_fen("8/8/2P5/3N4/2P5/8/8/8 w - - 0 1");
//     const Square knight_square(3, 4); // d5
//     const Piece knight('N');
//     game.generate_knight_moves(moves, knight_square);

//     // Should NOT include moves to c6 or c4
//     EXPECT_FALSE(contains_move(knight_square, Square(2, 6), knight));
//     EXPECT_FALSE(contains_move(knight_square, Square(2, 2), knight));
// }

// TEST_F(KnightMoveTest, ThrowsIfNoPieceAtSquare) {
//     game.from_fen("8/8/8/8/8/8/8/8 w - - 0 1");
//     const Square empty_square(4, 4); // e5

//     EXPECT_THROW(game.generate_knight_moves(moves, empty_square), std::runtime_error);
// }

// TEST_F(KnightMoveTest, ThrowsIfNotAKnight) {
//     game.from_fen("8/8/8/3P4/8/8/8/8 w - - 0 1");
//     const Square pawn_square(3, 4); // d5

//     EXPECT_THROW(game.generate_knight_moves(moves, pawn_square), std::runtime_error);
// }