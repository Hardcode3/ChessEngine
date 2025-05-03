#include <gtest/gtest.h>
#include <chess_engine/game.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <chess_engine/move.hpp>
#include <fixture_move_test.cpp>

using namespace chess;

TEST_F(MoveTest, CenterKnightHasEightMoves) {
    game.from_fen("8/8/8/3N4/8/8/8/8 w - - 0 1");
    const Square knight_square(3, 4); // d5
    const Piece knight('N');
    game.generate_knight_moves(moves, knight_square);

    EXPECT_EQ(moves.size(), 8);

    EXPECT_TRUE(contains_move(knight_square, Square(4, 6), knight)); // d5e7
    EXPECT_TRUE(contains_move(knight_square, Square(2, 6), knight)); // d5c7
    EXPECT_TRUE(contains_move(knight_square, Square(1, 5), knight)); // d5b6
    EXPECT_TRUE(contains_move(knight_square, Square(1, 3), knight)); // d5b4
    EXPECT_TRUE(contains_move(knight_square, Square(2, 2), knight)); // d5c3
    EXPECT_TRUE(contains_move(knight_square, Square(4, 2), knight)); // d5e3
    EXPECT_TRUE(contains_move(knight_square, Square(5, 3), knight)); // d5f4
    EXPECT_TRUE(contains_move(knight_square, Square(5, 5), knight)); // d5f6
}
