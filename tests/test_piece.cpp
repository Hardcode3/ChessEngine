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

TEST(PieceTest, IsValidForValidPieces) {
    for (const auto& test_case : VALID_PIECES) {
        if (test_case.type != Piece::Type::EMPTY) {  // Skip empty pieces
            Piece piece(test_case.type, test_case.color);
            EXPECT_TRUE(piece.is_valid()) << "Piece " << test_case.fen_char << " should be valid";
        }
    }
}

TEST(PieceTest, IsValidForEmptyPiece) {
    Piece empty_piece;
    EXPECT_FALSE(empty_piece.is_valid());
}

TEST(PieceTest, IsValidForEmptyTypeWithColor) {
    Piece invalid_type(Piece::Type::EMPTY, Piece::Color::WHITE);
    EXPECT_FALSE(invalid_type.is_valid());
}

TEST(PieceTest, IsValidForValidTypeWithoutColor) {
    Piece invalid_color(Piece::Type::PAWN, Piece::Color::NO_COLOR);
    EXPECT_FALSE(invalid_color.is_valid());
}

TEST(PieceTest, IsValidForInvalidCharacters) {
    for (char c : INVALID_CHARS) {
        Piece piece(c);
        EXPECT_FALSE(piece.is_valid()) << "Invalid character " << c << " should create invalid piece";
    }
}

TEST(PieceTest, EqualityForSamePieces) {
    Piece white_pawn1(Piece::Type::PAWN, Piece::Color::WHITE);
    Piece white_pawn2(Piece::Type::PAWN, Piece::Color::WHITE);
    EXPECT_TRUE(white_pawn1 == white_pawn2);
    EXPECT_FALSE(white_pawn1 != white_pawn2);
}

TEST(PieceTest, EqualityForDifferentTypes) {
    Piece white_pawn(Piece::Type::PAWN, Piece::Color::WHITE);
    Piece white_knight(Piece::Type::KNIGHT, Piece::Color::WHITE);
    EXPECT_FALSE(white_pawn == white_knight);
    EXPECT_TRUE(white_pawn != white_knight);
}

TEST(PieceTest, EqualityForDifferentColors) {
    Piece white_pawn(Piece::Type::PAWN, Piece::Color::WHITE);
    Piece black_pawn(Piece::Type::PAWN, Piece::Color::BLACK);
    EXPECT_FALSE(white_pawn == black_pawn);
    EXPECT_TRUE(white_pawn != black_pawn);
}

TEST(PieceTest, EqualityForEmptyPieces) {
    Piece empty1;
    Piece empty2;
    EXPECT_TRUE(empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);
}

TEST(PieceTest, EqualityForEmptyVsNonEmpty) {
    Piece white_pawn(Piece::Type::PAWN, Piece::Color::WHITE);
    Piece empty;
    EXPECT_FALSE(white_pawn == empty);
    EXPECT_TRUE(white_pawn != empty);
}
