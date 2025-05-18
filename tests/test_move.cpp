#include <gtest/gtest.h>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

using namespace chess;

namespace {
// Helper function to create a valid piece
Piece create_piece(Piece::Type type, Piece::Color color) {
    return Piece(type, color);
}

// Test cases for valid moves
struct MoveTestCase {
    std::string from;
    std::string to;
    Piece::Type piece_type;
    Piece::Color piece_color;
    std::string expected_uci;
    bool is_promotion;
};

const std::vector<MoveTestCase> VALID_MOVES = {
    {"e2", "e4", Piece::Type::PAWN,   Piece::Color::WHITE, "e2e4", false},
    {"g1", "f3", Piece::Type::KNIGHT, Piece::Color::WHITE, "g1f3", false},
    {"e7", "e5", Piece::Type::PAWN,   Piece::Color::BLACK, "e7e5", false},
    {"e1", "g1", Piece::Type::KING,   Piece::Color::WHITE, "e1g1", false},
    {"a7", "a8", Piece::Type::PAWN,   Piece::Color::BLACK, "a7a8q", true}  // Promotion
};

// Test cases for invalid UCI moves
const std::vector<std::string> INVALID_UCI_MOVES = {
    "",          // Empty
    "e2",        // Too short
    "e2e4e6",    // Too long
    "e2e4x",     // Invalid promotion
    "i1a1",      // Invalid file
    "a9a1",      // Invalid rank
    "a1a1",      // Same square
    "a1b2c3"     // Too many characters
};
} // namespace

TEST(MoveTest, ConstructorFromSquaresAndPiece) {
    for (const auto& test_case : VALID_MOVES) {
        Square from(test_case.from);
        Square to(test_case.to);
        Piece piece(test_case.piece_type, test_case.piece_color);

        Move move(from, to, piece);

        EXPECT_EQ(move.get_from(), from);
        EXPECT_EQ(move.get_to(), to);
        EXPECT_EQ(move.get_piece(), piece);
    }
}

TEST(MoveTest, ConstructorFromSquaresAndPieceInvalid) {
    // Test with invalid piece
    Square from("e2");
    Square to("e4");
    Piece invalid_piece;  // Default constructor creates empty piece

    EXPECT_THROW(Move(from, to, invalid_piece), std::invalid_argument);
}

TEST(MoveTest, SetAndGetCaptured) {
    Square from("e2");
    Square to("e4");
    Piece piece(Piece::Type::PAWN, Piece::Color::WHITE);
    Move move(from, to, piece);

    Piece captured(Piece::Type::PAWN, Piece::Color::BLACK);
    move.set_captured(captured);
    EXPECT_TRUE(move.is_capture());
    EXPECT_EQ(*move.get_captured(), captured);
}

TEST(MoveTest, SetAndGetCastling) {
    Square from("e1");
    Square to("g1");
    Piece piece(Piece::Type::KING, Piece::Color::WHITE);
    Move move(from, to, piece);

    Square rook_from("h1");
    Square rook_to("f1");
    move.set_castling(rook_from, rook_to);
    EXPECT_TRUE(move.is_castling());
    EXPECT_EQ(move.get_castling()->rook_from, rook_from);
    EXPECT_EQ(move.get_castling()->rook_to, rook_to);
}

TEST(MoveTest, SetAndGetPromotion) {
    Square from("a7");
    Square to("a8");
    Piece piece(Piece::Type::PAWN, Piece::Color::BLACK);
    Move move(from, to, piece);

    Piece promoted_to(Piece::Type::QUEEN, Piece::Color::BLACK);
    move.set_promotion(promoted_to);
    EXPECT_TRUE(move.is_promotion());
    EXPECT_EQ(move.get_promoted_to(), promoted_to);
}

TEST(MoveTest, PawnMoveQueries) {
    Move pawn_move(Square("e2"), Square("e4"), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    EXPECT_TRUE(pawn_move.is_pawn_move());
    EXPECT_TRUE(pawn_move.is_pawn_double_push());
    EXPECT_FALSE(pawn_move.is_castling());
    EXPECT_FALSE(pawn_move.is_promotion());
    EXPECT_TRUE(pawn_move.is_quiet());
}

TEST(MoveTest, CaptureMoveQueries) {
    Move capture_move(Square("e4"), Square("d5"), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    capture_move.set_captured(Piece(Piece::Type::PAWN, Piece::Color::BLACK));
    EXPECT_TRUE(capture_move.is_capture());
    EXPECT_FALSE(capture_move.is_quiet());
}

TEST(MoveTest, EnPassantMoveQueries) {
    Move en_passant(Square("e5"), Square("d6"), Piece(Piece::Type::PAWN, Piece::Color::WHITE));
    en_passant.set_captured(Piece());  // Empty piece for en passant
    EXPECT_TRUE(en_passant.is_en_passant());
}

TEST(MoveTest, CastlingMoveQueries) {
    Move castle_move(Square("e1"), Square("g1"), Piece(Piece::Type::KING, Piece::Color::WHITE));
    castle_move.set_castling(Square("h1"), Square("f1"));
    EXPECT_TRUE(castle_move.is_castling());
    EXPECT_TRUE(castle_move.is_king_side_castle());
    EXPECT_FALSE(castle_move.is_queen_side_castle());
}

TEST(MoveTest, PromotionMoveQueries) {
    Move promotion_move(Square("a7"), Square("a8"), Piece(Piece::Type::PAWN, Piece::Color::BLACK));
    promotion_move.set_promotion(Piece(Piece::Type::QUEEN, Piece::Color::BLACK));
    EXPECT_TRUE(promotion_move.is_promotion());
}

TEST(MoveTest, ToUCI) {
    for (const auto& test_case : VALID_MOVES) {
        Square from(test_case.from);
        Square to(test_case.to);
        Piece piece(test_case.piece_type, test_case.piece_color);
        Move move(from, to, piece);

        if (test_case.is_promotion) {
            move.set_promotion(Piece(test_case.expected_uci[4]));
        }

        EXPECT_EQ(move.to_uci(), test_case.expected_uci);
        EXPECT_EQ(move.is_promotion(), test_case.is_promotion);
    }
}
