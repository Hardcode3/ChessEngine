#include <algorithm>
#include <chess_engine/board.hpp>

namespace chess {

Piece Board::get_piece(const Square& square) const {
  const uint8_t index = square.rank * BOARD_SIZE + square.file;
  return board[index];
}
void Board::set_piece(const Square& square, Piece piece) {
  const int index = square.rank * BOARD_SIZE + square.file;
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
}  // namespace chess
