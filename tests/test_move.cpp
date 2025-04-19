#include <gtest/gtest.h>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

using namespace chess;

TEST(MoveTest, BasicConstructor) {
    Square from(0, 0);  // a1
    Square to(1, 1);    // b2
    Piece piece = Piece::PAWN;

    Move move(from, to, piece);

    EXPECT_EQ(move.from.file, 0);
    EXPECT_EQ(move.from.rank, 0);
    EXPECT_EQ(move.to.file, 1);
    EXPECT_EQ(move.to.rank, 1);
    EXPECT_EQ(move.piece, Piece::PAWN);
    EXPECT_EQ(move.captured, Piece::EMPTY);
    EXPECT_FALSE(move.is_promotion);
    EXPECT_EQ(move.promotion_piece, Piece::EMPTY);
    EXPECT_FALSE(move.is_castling);
}

TEST(MoveTest, ConstructorWithCapture) {
    Square from(0, 0);  // a1
    Square to(1, 1);    // b2
    Piece piece = Piece::PAWN;
    Piece captured = Piece::PAWN;

    Move move(from, to, piece, captured);

    EXPECT_EQ(move.from.file, 0);
    EXPECT_EQ(move.from.rank, 0);
    EXPECT_EQ(move.to.file, 1);
    EXPECT_EQ(move.to.rank, 1);
    EXPECT_EQ(move.piece, Piece::PAWN);
    EXPECT_EQ(move.captured, Piece::PAWN);
    EXPECT_FALSE(move.is_promotion);
    EXPECT_EQ(move.promotion_piece, Piece::EMPTY);
    EXPECT_FALSE(move.is_castling);
}

TEST(MoveTest, ConstructorWithPromotion) {
    Square from(0, 6);  // a7
    Square to(0, 7);    // a8
    Piece piece = Piece::PAWN;
    Piece captured = Piece::EMPTY;
    bool is_promotion = true;
    Piece promotion_piece = Piece::QUEEN;

    Move move(from, to, piece, captured, is_promotion, promotion_piece);

    EXPECT_EQ(move.from.file, 0);
    EXPECT_EQ(move.from.rank, 6);
    EXPECT_EQ(move.to.file, 0);
    EXPECT_EQ(move.to.rank, 7);
    EXPECT_EQ(move.piece, Piece::PAWN);
    EXPECT_EQ(move.captured, Piece::EMPTY);
    EXPECT_TRUE(move.is_promotion);
    EXPECT_EQ(move.promotion_piece, Piece::QUEEN);
    EXPECT_FALSE(move.is_castling);
}

TEST(MoveTest, ToUciBasicMove) {
    Square from(0, 0);  // a1
    Square to(1, 1);    // b2
    Piece piece = Piece::PAWN;

    Move move(from, to, piece);

    EXPECT_EQ(move.to_uci(), "a1b2");
}

TEST(MoveTest, ToUciQueenPromotion) {
    Square from(0, 6);  // a7
    Square to(0, 7);    // a8
    Piece piece = Piece::PAWN;

    Move queen_promotion(from, to, piece, Piece::EMPTY, true, Piece::QUEEN);
    EXPECT_EQ(queen_promotion.to_uci(), "a7a8q");
}

TEST(MoveTest, ToUciRookPromotion) {
    Square from(0, 6);  // a7
    Square to(0, 7);    // a8
    Piece piece = Piece::PAWN;

    Move rook_promotion(from, to, piece, Piece::EMPTY, true, Piece::ROOK);
    EXPECT_EQ(rook_promotion.to_uci(), "a7a8r");
}

TEST(MoveTest, ToUciBishopPromotion) {
    Square from(0, 6);  // a7
    Square to(0, 7);    // a8
    Piece piece = Piece::PAWN;

    Move bishop_promotion(from, to, piece, Piece::EMPTY, true, Piece::BISHOP);
    EXPECT_EQ(bishop_promotion.to_uci(), "a7a8b");
}

TEST(MoveTest, ToUciKnightPromotion) {
    Square from(0, 6);  // a7
    Square to(0, 7);    // a8
    Piece piece = Piece::PAWN;

    Move knight_promotion(from, to, piece, Piece::EMPTY, true, Piece::KNIGHT);
    EXPECT_EQ(knight_promotion.to_uci(), "a7a8n");
}
