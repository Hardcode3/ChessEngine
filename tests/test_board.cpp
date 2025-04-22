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

struct PieceTestCase {
    Square square;
    Piece piece;
    bool should_succeed;
    std::string test_name;
};


TEST_F(BoardTest, InitializationProvidesEmptyBoard) {
    // Test that board is properly initialized with empty pieces
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Square square(file, rank);
            EXPECT_TRUE(board.is_empty(square)) << "Square " << file << ", " << rank << " should be empty but is not";
        }
    }
}

TEST_F(BoardTest, SetPiecesAndClearBoard) {
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

TEST_F(BoardTest, SetAndGetPiecesByColor) {
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

TEST_F(BoardTest, SetAndGetPiecesByTypeAndColor) {
    std::vector<PieceTestCase> test_cases = {
        {Square(0, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE), true, "White pawn at a1"},
        {Square(7, 7), Piece(Piece::Type::KING, Piece::Color::BLACK), true, "Black king at h8"},
        {Square(4, 4), Piece(Piece::Type::QUEEN, Piece::Color::WHITE), true, "White queen at e5"},
        {Square(1, 6), Piece(Piece::Type::KNIGHT, Piece::Color::BLACK), true, "Black knight at b7"},
        {Square(3, 2), Piece(Piece::Type::BISHOP, Piece::Color::WHITE), true, "White bishop at d3"},
        {Square(5, 1), Piece(Piece::Type::ROOK, Piece::Color::BLACK), true, "Black rook at f2"}
    };

    for (const auto& test_case : test_cases) {
        board.set_piece(test_case.square, test_case.piece);

        EXPECT_FALSE(board.is_empty(test_case.square))
            << "Test failed for " << test_case.test_name;
        EXPECT_EQ(board.get_piece(test_case.square).type, test_case.piece.type)
            << "Type mismatch for " << test_case.test_name;
        EXPECT_EQ(board.get_piece(test_case.square).color, test_case.piece.color)
            << "Color mismatch for " << test_case.test_name;
    }
}

TEST_F(BoardTest, SetGetOnEdgeCases) {
    std::vector<PieceTestCase> edge_cases = {
        {Square(0, 0), Piece(Piece::Type::KING, Piece::Color::WHITE), true, "White king at a1"},
        {Square(7, 0), Piece(Piece::Type::ROOK, Piece::Color::BLACK), true, "Black rook at h1"},
        {Square(0, 7), Piece(Piece::Type::BISHOP, Piece::Color::WHITE), true, "White bishop at a8"},
        {Square(7, 7), Piece(Piece::Type::KNIGHT, Piece::Color::BLACK), true, "Black knight at h8"}
    };

    for (const auto& test_case : edge_cases) {
        board.set_piece(test_case.square, test_case.piece);

        EXPECT_FALSE(board.is_empty(test_case.square))
            << "Test failed for " << test_case.test_name;
        EXPECT_EQ(board.get_piece(test_case.square).type, test_case.piece.type)
            << "Type mismatch for " << test_case.test_name;
        EXPECT_EQ(board.get_piece(test_case.square).color, test_case.piece.color)
            << "Color mismatch for " << test_case.test_name;
    }
}

TEST_F(BoardTest, SetOverridesPieces) {
    // First place a white pawn
    Square square(3, 3);
    Piece white_pawn(Piece::Type::PAWN, Piece::Color::WHITE);
    board.set_piece(square, white_pawn);

    // Verify initial placement
    EXPECT_EQ(board.get_piece(square).type, Piece::Type::PAWN);
    EXPECT_EQ(board.get_piece(square).color, Piece::Color::WHITE);

    // Overwrite with black queen
    Piece black_queen(Piece::Type::QUEEN, Piece::Color::BLACK);
    board.set_piece(square, black_queen);

    // Verify overwrite
    EXPECT_EQ(board.get_piece(square).type, Piece::Type::QUEEN);
    EXPECT_EQ(board.get_piece(square).color, Piece::Color::BLACK);
}

TEST_F(BoardTest, GetPieceBySquare) {
    // Set up pieces in asymmetric patterns
    std::vector<std::pair<Square, Piece>> piece_placements = {
        // Corners and edges
        {Square(0, 0), Piece(Piece::Type::ROOK, Piece::Color::WHITE)},
        {Square(7, 0), Piece(Piece::Type::ROOK, Piece::Color::WHITE)},
        {Square(0, 7), Piece(Piece::Type::ROOK, Piece::Color::BLACK)},
        {Square(7, 7), Piece(Piece::Type::ROOK, Piece::Color::BLACK)},

        // Center and off-center positions
        {Square(3, 4), Piece(Piece::Type::QUEEN, Piece::Color::WHITE)},
        {Square(4, 3), Piece(Piece::Type::QUEEN, Piece::Color::BLACK)},

        // Random asymmetric positions
        {Square(2, 5), Piece(Piece::Type::KNIGHT, Piece::Color::WHITE)},
        {Square(5, 2), Piece(Piece::Type::KNIGHT, Piece::Color::BLACK)},
        {Square(1, 6), Piece(Piece::Type::BISHOP, Piece::Color::WHITE)},
        {Square(6, 1), Piece(Piece::Type::BISHOP, Piece::Color::BLACK)},

        // Edge positions (not corners)
        {Square(0, 3), Piece(Piece::Type::PAWN, Piece::Color::WHITE)},
        {Square(3, 0), Piece(Piece::Type::PAWN, Piece::Color::WHITE)},
        {Square(7, 4), Piece(Piece::Type::PAWN, Piece::Color::BLACK)},
        {Square(4, 7), Piece(Piece::Type::PAWN, Piece::Color::BLACK)}
    };

    // Place all pieces
    for (const auto& [square, piece] : piece_placements) {
        board.set_piece(square, piece);
    }

    // Test getting each piece
    for (const auto& [square, expected_piece] : piece_placements) {
        Piece retrieved_piece = board.get_piece(square);
        EXPECT_EQ(retrieved_piece.type, expected_piece.type)
            << "Type mismatch at square (" << square.file << ", " << square.rank << ")";
        EXPECT_EQ(retrieved_piece.color, expected_piece.color)
            << "Color mismatch at square (" << square.file << ", " << square.rank << ")";
    }
}

TEST_F(BoardTest, GetPieceFromEmptySquare) {
    // Test getting pieces from empty squares in various patterns
    std::vector<Square> empty_squares = {
        // Empty squares around corners
        Square(1, 0), Square(0, 1),
        Square(6, 0), Square(7, 1),
        Square(0, 6), Square(1, 7),
        Square(6, 7), Square(7, 6),

        // Empty squares in center
        Square(3, 3), Square(4, 4),

        // Empty squares in asymmetric positions
        Square(2, 4), Square(5, 3),
        Square(1, 5), Square(6, 2)
    };

    for (const auto& square : empty_squares) {
        board.set_piece(square, Piece(Piece::Type::EMPTY, Piece::Color::NO_COLOR));
        Piece retrieved_piece = board.get_piece(square);
        EXPECT_EQ(retrieved_piece.type, Piece::Type::EMPTY)
            << "Expected empty square at (" << square.file << ", " << square.rank << ")";
        EXPECT_EQ(retrieved_piece.color, Piece::Color::NO_COLOR)
            << "Expected non colored square at (" << square.file << ", " << square.rank << ")";
    }
}
