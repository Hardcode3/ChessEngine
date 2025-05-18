#pragma once

#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <optional>

namespace chess {

struct GameState {
  bool white_can_castle_kingside = false;
  bool white_can_castle_queenside = false;
  bool black_can_castle_kingside = false;
  bool black_can_castle_queenside = false;
  Piece::Color side_to_move = Piece::Color::WHITE;
  std::optional<Square> en_passant_square;
  uint16_t halfmove_clock = 0;
  uint16_t fullmove_number = 1;
};

}  // namespace chess