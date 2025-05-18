#pragma once

#include <array>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <vector>

namespace chess {

constexpr uint8_t BOARD_SIZE = 8;

class Board {
 public:
  Board() : board{} {}

  Piece get_piece(const Square& square) const;
  Piece get_piece(const uint8_t& file, const uint8_t& rank) const;
  void set_piece(const Square& square, Piece piece);
  void set_piece(const uint8_t& file, const uint8_t& rank, Piece piece);
  std::vector<Square> get_pieces(Piece::Type type, Piece::Color color) const;
  std::vector<Square> get_pieces(Piece::Color color) const;

  void clear();

  bool is_empty(Square square) const;

  friend std::ostream& operator<<(std::ostream& os, const Board& board);

 private:
  std::array<Piece, BOARD_SIZE * BOARD_SIZE> board;
};

}  // namespace chess