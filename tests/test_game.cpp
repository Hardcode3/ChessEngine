#include <gtest/gtest.h>
#include <chess_engine/game.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

using namespace chess;

struct FenTestCase {
    std::string fen;
    std::string description;
    std::vector<std::pair<Square, Piece>> expected_pieces;
    Piece::Color expected_side_to_move;
    bool expected_white_kingside;
    bool expected_white_queenside;
    bool expected_black_kingside;
    bool expected_black_queenside;
    std::optional<Square> expected_en_passant;
    int expected_halfmove_clock;
    int expected_fullmove_number;
};

class GameTest : public ::testing::Test {
protected:
    Game game;
    std::vector<FenTestCase> test_cases = {
        {
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            "Standard starting position",
            {
                // Black pieces on ranks 8 and 7 (y = 0, 1)
                {Square(0, 0), Piece('R')}, {Square(1, 0), Piece('N')}, {Square(2, 0), Piece('B')},
                {Square(3, 0), Piece('Q')}, {Square(4, 0), Piece('K')}, {Square(5, 0), Piece('B')},
                {Square(6, 0), Piece('N')}, {Square(7, 0), Piece('R')},
                {Square(0, 1), Piece('P')}, {Square(1, 1), Piece('P')}, {Square(2, 1), Piece('P')},
                {Square(3, 1), Piece('P')}, {Square(4, 1), Piece('P')}, {Square(5, 1), Piece('P')},
                {Square(6, 1), Piece('P')}, {Square(7, 1), Piece('P')},

                // White pieces on ranks 2 and 1 (y = 6, 7)
                {Square(0, 6), Piece('p')}, {Square(1, 6), Piece('p')}, {Square(2, 6), Piece('p')},
                {Square(3, 6), Piece('p')}, {Square(4, 6), Piece('p')}, {Square(5, 6), Piece('p')},
                {Square(6, 6), Piece('p')}, {Square(7, 6), Piece('p')},
                {Square(0, 7), Piece('r')}, {Square(1, 7), Piece('n')}, {Square(2, 7), Piece('b')},
                {Square(3, 7), Piece('q')}, {Square(4, 7), Piece('k')}, {Square(5, 7), Piece('b')},
                {Square(6, 7), Piece('n')}, {Square(7, 7), Piece('r')}
            },
            Piece::Color::WHITE,
            true, true, true, true,
            std::nullopt,
            0, 1
        },
        {
            "8/8/8/8/8/8/8/8 w - - 0 1",
            "Empty board",
            {},
            Piece::Color::WHITE,
            false, false, false, false,
            std::nullopt,
            0, 1
        },
        {
            "4k3/8/8/8/8/8/8/4K3 b - - 5 10",
            "Kings only, black to move",
            {
                {Square(4, 7), Piece('k')},
                {Square(4, 0), Piece('K')}
            },
            Piece::Color::BLACK,
            false, false, false, false,
            std::nullopt,
            5, 10
        },
        {
            "r3k2r/8/8/8/8/8/8/R3K2R w KQkq e3 0 1",
            "Castling setup with en passant",
            {
                {Square(0, 0), Piece('R')},  // a8
                {Square(4, 0), Piece('K')},  // e8
                {Square(7, 0), Piece('R')},  // h8
                {Square(0, 7), Piece('r')},  // a1
                {Square(4, 7), Piece('k')},  // e1
                {Square(7, 7), Piece('r')}   // h1
            },
            Piece::Color::WHITE,
            true, true, true, true,
            Square("e3"),
            0, 1
        }
    };
};

TEST_F(GameTest, LoadFenPositions) {
    for (const auto& test_case : test_cases) {
        SCOPED_TRACE(test_case.description);

        game.from_fen(test_case.fen);

        // Check all expected pieces are in place
        for (const auto& [square, expected_piece] : test_case.expected_pieces) {
            Piece actual_piece = game.get_piece(square);
            EXPECT_EQ(actual_piece.type, expected_piece.type)
                << "Piece type mismatch at " << square.to_string() << " expected: " << expected_piece.to_char() << ", got: " << actual_piece.to_char();
            EXPECT_EQ(actual_piece.color, expected_piece.color)
                << "Piece color mismatch at " << square.to_string() << " expected: " << expected_piece.to_char() << ", got: " << actual_piece.to_char();
        }

        // Check game state
        EXPECT_EQ(game.get_side_to_move(), test_case.expected_side_to_move);

        const auto& state = game.get_state();
        EXPECT_EQ(state.white_can_castle_kingside, test_case.expected_white_kingside);
        EXPECT_EQ(state.white_can_castle_queenside, test_case.expected_white_queenside);
        EXPECT_EQ(state.black_can_castle_kingside, test_case.expected_black_kingside);
        EXPECT_EQ(state.black_can_castle_queenside, test_case.expected_black_queenside);

        if (test_case.expected_en_passant) {
            EXPECT_EQ(state.en_passant_square, test_case.expected_en_passant);
        } else {
            EXPECT_EQ(state.en_passant_square, std::nullopt);
        }

        EXPECT_EQ(state.halfmove_clock, test_case.expected_halfmove_clock);
        EXPECT_EQ(state.fullmove_number, test_case.expected_fullmove_number);
    }
}

TEST_F(GameTest, GetFenMatchesSetFen) {
    for (const auto& test_case : test_cases) {
        SCOPED_TRACE(test_case.description);

        game.from_fen(test_case.fen);
        std::string output_fen = game.get_fen();

        // Compare the output FEN with the input FEN
        EXPECT_EQ(output_fen, test_case.fen)
            << "FEN mismatch\nInput:  " << test_case.fen
            << "\nOutput: " << output_fen;

    }
}