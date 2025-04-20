#include <gtest/gtest.h>
#include <chess_engine/board.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

using namespace chess;

TEST(BoardTest, DefaultConstructor) {
    Board board;

    // Test that all squares are empty
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            EXPECT_EQ(board.get_piece(Square(file, rank)), Piece::EMPTY);
        }
    }

    // Test initial game state
    EXPECT_EQ(board.get_side_to_move(), Color::WHITE);
}

TEST(BoardTest, LoadFENStartingPositionWhitePieces) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    // Test white back rank pieces
    EXPECT_EQ(board.get_piece(Square(0, 0)), Piece::ROOK);
    EXPECT_EQ(board.get_color(Square(0, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(1, 0)), Piece::KNIGHT);
    EXPECT_EQ(board.get_color(Square(1, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(2, 0)), Piece::BISHOP);
    EXPECT_EQ(board.get_color(Square(2, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(3, 0)), Piece::QUEEN);
    EXPECT_EQ(board.get_color(Square(3, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(4, 0)), Piece::KING);
    EXPECT_EQ(board.get_color(Square(4, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(5, 0)), Piece::BISHOP);
    EXPECT_EQ(board.get_color(Square(5, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(6, 0)), Piece::KNIGHT);
    EXPECT_EQ(board.get_color(Square(6, 0)), Color::WHITE);
    EXPECT_EQ(board.get_piece(Square(7, 0)), Piece::ROOK);
    EXPECT_EQ(board.get_color(Square(7, 0)), Color::WHITE);
}

TEST(BoardTest, LoadFENStartingPositionBlackPieces) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    // Test black back rank pieces
    EXPECT_EQ(board.get_piece(Square(0, 7)), Piece::ROOK);
    EXPECT_EQ(board.get_color(Square(0, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(1, 7)), Piece::KNIGHT);
    EXPECT_EQ(board.get_color(Square(1, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(2, 7)), Piece::BISHOP);
    EXPECT_EQ(board.get_color(Square(2, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(3, 7)), Piece::QUEEN);
    EXPECT_EQ(board.get_color(Square(3, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(4, 7)), Piece::KING);
    EXPECT_EQ(board.get_color(Square(4, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(5, 7)), Piece::BISHOP);
    EXPECT_EQ(board.get_color(Square(5, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(6, 7)), Piece::KNIGHT);
    EXPECT_EQ(board.get_color(Square(6, 7)), Color::BLACK);
    EXPECT_EQ(board.get_piece(Square(7, 7)), Piece::ROOK);
    EXPECT_EQ(board.get_color(Square(7, 7)), Color::BLACK);
}

TEST(BoardTest, LoadFENStartingPositionEmptySquares) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    // Test empty squares
    for (int rank = 2; rank < 6; ++rank) {
        for (int file = 0; file < 8; ++file) {
            EXPECT_EQ(board.get_piece(Square(file, rank)), Piece::EMPTY);
        }
    }

    // Test initial game state
    EXPECT_EQ(board.get_side_to_move(), Color::WHITE);
}

TEST(BoardTest, GenerateLegalMovesFromStartingPosition) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    auto moves = board.generate_legal_moves();

    // Starting position should have 20 legal moves
    EXPECT_EQ(moves.size(), 20);

    // Test a given move
    bool found_e2e4 = false;
    for (const auto& move : moves) {
        if (move.to_uci() == "e2e4") {
            found_e2e4 = true;
            break;
        }
    }
    EXPECT_TRUE(found_e2e4);
}

TEST(BoardTest, MakeMoveFromStartingPosition) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    // Make e2e4
    const Square from(4, 1);
    const Square to(4, 3);

    Move move(from, to, Piece::PAWN);
    board.make_move(move);

    // Test piece moved
    EXPECT_EQ(board.get_piece(from), Piece::EMPTY);
    EXPECT_EQ(board.get_piece(to), Piece::PAWN);

    // Test side to move changed
    EXPECT_EQ(board.get_side_to_move(), Color::BLACK);
}

TEST(BoardTest, EvaluatePositionIsNull) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    // Starting position should be equal
    EXPECT_EQ(board.evaluate(), 0);
}

TEST(BoardTest, EvaluatePositionAdvantageToWhite) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    Move capture(Square(0, 1), Square(0, 6), Piece::PAWN, Piece::PAWN);
    board.make_move(capture);

    // White should have a material advantage
    EXPECT_GT(board.evaluate(), 0);
}

TEST(BoardTest, UciToMove) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);
    Move move = board.uci_to_move("e2e4");

    EXPECT_EQ(move.from.file, 4);
    EXPECT_EQ(move.from.rank, 1);
    EXPECT_EQ(move.to.file, 4);
    EXPECT_EQ(move.to.rank, 3);
    EXPECT_EQ(move.piece, Piece::PAWN);
}

TEST(BoardTest, MoveToUci) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);
    Move move(Square(4, 1), Square(4, 3), Piece::PAWN);
    std::string uci = board.move_to_uci(move);

    EXPECT_EQ(uci, "e2e4");
}

TEST(BoardTest, StartingPositionIsNotGameOver) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    EXPECT_FALSE(board.is_game_over());
}

TEST(BoardTest, StalematePositionIsGameOver) {
    Board board;
    // This is a classic stalemate position where black has no legal moves
    board.load_fen("k7/8/8/8/8/8/1R6/7K b - - 0 1");

    EXPECT_TRUE(board.is_game_over());
}

TEST(BoardTest, CheckmatePositionIsGameOver) {
    Board board;
    // This is a classic checkmate position (Fool's mate)
    board.load_fen("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 0 1");

    EXPECT_TRUE(board.is_game_over());
}

TEST(BoardTest, InsufficientMaterialIsGameOver) {
    Board board;
    // This is a position with just two kings (insufficient material)
    board.load_fen("k7/8/8/8/8/8/8/7K w - - 0 1");

    EXPECT_TRUE(board.is_game_over());
}

TEST(BoardTest, SafeSquares) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);

    EXPECT_TRUE(board.is_square_safe(Square(4, 0), Color::WHITE)); // White king
    EXPECT_TRUE(board.is_square_safe(Square(4, 7), Color::BLACK)); // Black king
}

TEST(BoardTest, UnsafeSquares) {
    Board board;
    board.load_fen(Board::STARTING_POSITION);
    Move move(Square(4, 1), Square(4, 3), Piece::PAWN);
    board.make_move(move);

    EXPECT_FALSE(board.is_square_safe(Square(4, 4), Color::BLACK)); // Square controlled by white pawn
}
