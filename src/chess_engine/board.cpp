#include <algorithm>
#include <chess_engine/board.hpp>
#include <ostream>

namespace chess {

Piece Board::get_piece(const Square& square) const {
  const uint8_t index = square.rank * BOARD_SIZE + square.file;
  return board[index];
}
Piece Board::get_piece(const uint8_t& file, const uint8_t& rank) const {
  const uint8_t index = rank * BOARD_SIZE + file;
  return board[index];
}
void Board::set_piece(const Square& square, Piece piece) {
  const uint8_t index = square.rank * BOARD_SIZE + square.file;
  board[index] = piece;
}
void Board::set_piece(const uint8_t& file, const uint8_t& rank, Piece piece) {
  const uint8_t index = rank * BOARD_SIZE + file;
  board[index] = piece;
}

void Board::clear() { std::fill(board.begin(), board.end(), Piece()); }

bool Board::is_empty(Square square) const { return get_piece(square).type == Piece::Type::EMPTY; }

std::vector<Square> Board::get_pieces(Piece::Type type, Piece::Color color) const {
  std::vector<Square> result;
  for (uint8_t i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
    if (board[i].type == type && board[i].color == color) {
      result.push_back(Square(i / BOARD_SIZE, i % BOARD_SIZE));
    }
  }
  return result;
}

std::vector<Square> Board::get_pieces(Piece::Color color) const {
  std::vector<Square> result;
  for (uint8_t i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
    if (board[i].color == color) {
      result.push_back(Square(i / BOARD_SIZE, i % BOARD_SIZE));
    }
  }
  return result;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    os << "  +--------------------------+" << '\n';
    for(int rank = 7; rank >= 0; --rank) {
        os << rank + 1 << " |  ";
        for(int file = 0; file < BOARD_SIZE; ++file) {
            char piece_char = board.get_piece(Square(file, rank)).to_char();
            os << piece_char << "  ";
        }
        os << '|' << '\n';
    }
    os << "  +--------------------------+" << '\n';
    os << "     a  b  c  d  e  f  g  h" << '\n';
    return os;
}

}  // namespace chess
