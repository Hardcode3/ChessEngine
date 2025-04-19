#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstdint>

#include <chess_engine/square.hpp>
#include <chess_engine/piece.hpp>


namespace chess {

struct Move {
    Square from;
    Square to;
    Piece piece;
    Piece captured;
    bool is_promotion;
    Piece promotion_piece;

    Move(Square f, Square t, Piece p)
        : from(f), to(t), piece(p), captured(Piece::EMPTY),
          is_promotion(false), promotion_piece(Piece::EMPTY) {}

    std::string to_uci() const;
};

} // namespace chess