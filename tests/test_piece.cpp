#include <gtest/gtest.h>
#include <chess_engine/piece.hpp>

using namespace chess;

namespace {
// Shared test data structure
struct PieceTestCase {
    char fen_char;
    Piece::Type type;
    Piece::Color color;
};

// All valid piece test cases
const std::vector<PieceTestCase> VALID_PIECES = {
    {'P', Piece::Type::PAWN,   Piece::Color::WHITE},
    {'N', Piece::Type::KNIGHT, Piece::Color::WHITE},
    {'B', Piece::Type::BISHOP, Piece::Color::WHITE},
    {'R', Piece::Type::ROOK,   Piece::Color::WHITE},
    {'Q', Piece::Type::QUEEN,  Piece::Color::WHITE},
    {'K', Piece::Type::KING,   Piece::Color::WHITE},
    {'p', Piece::Type::PAWN,   Piece::Color::BLACK},
    {'n', Piece::Type::KNIGHT, Piece::Color::BLACK},
    {'b', Piece::Type::BISHOP, Piece::Color::BLACK},
    {'r', Piece::Type::ROOK,   Piece::Color::BLACK},
    {'q', Piece::Type::QUEEN,  Piece::Color::BLACK},
    {'k', Piece::Type::KING,   Piece::Color::BLACK},
    {'.', Piece::Type::EMPTY,  Piece::Color::NO_COLOR}
};

// Invalid piece characters
const std::vector<char> INVALID_CHARS = {
    'x', '1', '@', ' ', 'X', 'Z', '0', '9'
};
} // namespace

TEST(PieceTest, DefaultConstructor) {
    Piece piece;
    EXPECT_EQ(piece.type, Piece::Type::EMPTY);
    EXPECT_EQ(piece.color, Piece::Color::NO_COLOR);
    EXPECT_EQ(piece.to_char(), '.');
}

TEST(PieceTest, ConstructorFromTypeAndColor) {
    for (const auto& test_case : VALID_PIECES) {
        Piece piece(test_case.type, test_case.color);
        EXPECT_EQ(piece.type, test_case.type);
        EXPECT_EQ(piece.color, test_case.color);
        EXPECT_EQ(piece.to_char(), test_case.fen_char);
    }
}

TEST(PieceTest, ConstructorFromChar) {
    for (const auto& test_case : VALID_PIECES) {
        Piece piece(test_case.fen_char);
        EXPECT_EQ(piece.type, test_case.type);
        EXPECT_EQ(piece.color, test_case.color);
    }
}

TEST(PieceTest, ConstructorFromCharInvalid) {
    for (char c : INVALID_CHARS) {
        Piece piece(c);
        EXPECT_EQ(piece.type, Piece::Type::EMPTY);
        EXPECT_EQ(piece.color, Piece::Color::NO_COLOR);
        EXPECT_EQ(piece.to_char(), '.');
    }
}

TEST(PieceTest, ToChar) {
    for (const auto& test_case : VALID_PIECES) {
        Piece piece(test_case.type, test_case.color);
        EXPECT_EQ(piece.to_char(), test_case.fen_char);
    }
}
