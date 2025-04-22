#include <gtest/gtest.h>
#include <chess_engine/board.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

#include <iostream>

using namespace chess;

class BoardTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.clear();
    }
};

TEST_F(BoardTest, Initialization) {
    // Test that board is properly initialized with empty pieces
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Square square(file, rank);
            EXPECT_TRUE(board.is_empty(square)) << "Square " << file << ", " << rank << " should be empty but is not";
        }
    }
}

TEST_F(BoardTest, SetAndGetPiece) {
    Square square(0, 0);
    Piece white_pawn(Piece::Type::PAWN, Piece::Color::WHITE);

    board.set_piece(square, white_pawn);
    EXPECT_FALSE(board.is_empty(square));
    EXPECT_EQ(board.get_piece(square).type, Piece::Type::PAWN);
    EXPECT_EQ(board.get_piece(square).color, Piece::Color::WHITE);
}

TEST_F(BoardTest, ClearBoard) {
    // Set some pieces
    board.set_piece(Square(0, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    board.set_piece(Square(7, 7), Piece(Piece::Type::KING, Piece::Color::BLACK));

    // Clear the board
    board.clear();

    // Verify all squares are empty
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            EXPECT_TRUE(board.is_empty(Square(file, rank)));
        }
    }
}

TEST_F(BoardTest, GetPiecesByTypeAndColor) {
    // Set up some pieces
    board.set_piece(Square(0, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    board.set_piece(Square(1, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    board.set_piece(Square(0, 7), Piece(Piece::Type::PAWN, Piece::Color::BLACK));

    // Get white pawns
    auto white_pawns = board.get_pieces(Piece::Type::PAWN, Piece::Color::WHITE);
    EXPECT_EQ(white_pawns.size(), 2);

    // Get black pawns
    auto black_pawns = board.get_pieces(Piece::Type::PAWN, Piece::Color::BLACK);
    EXPECT_EQ(black_pawns.size(), 1);
}

TEST_F(BoardTest, GetPiecesByColor) {
    // Set up some pieces
    board.set_piece(Square(0, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    board.set_piece(Square(1, 0), Piece(Piece::Type::KNIGHT, Piece::Color::WHITE));
    board.set_piece(Square(0, 7), Piece(Piece::Type::PAWN, Piece::Color::BLACK));

    // Get all white pieces
    auto white_pieces = board.get_pieces(Piece::Color::WHITE);
    EXPECT_EQ(white_pieces.size(), 2);

    // Get all black pieces
    auto black_pieces = board.get_pieces(Piece::Color::BLACK);
    EXPECT_EQ(black_pieces.size(), 1);
}

TEST_F(BoardTest, EdgeCases) {
    // Test setting pieces on edge squares
    Square top_left(0, 0);
    Square top_right(7, 0);
    Square bottom_left(0, 7);
    Square bottom_right(7, 7);

    Piece white_king(Piece::Type::KING, Piece::Color::WHITE);

    board.set_piece(top_left, white_king);
    board.set_piece(top_right, white_king);
    board.set_piece(bottom_left, white_king);
    board.set_piece(bottom_right, white_king);

    EXPECT_EQ(board.get_piece(top_left).type, Piece::Type::KING);
    EXPECT_EQ(board.get_piece(top_right).type, Piece::Type::KING);
    EXPECT_EQ(board.get_piece(bottom_left).type, Piece::Type::KING);
    EXPECT_EQ(board.get_piece(bottom_right).type, Piece::Type::KING);
}
