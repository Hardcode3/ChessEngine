#include <chess_engine/game.hpp>
#include <format>
#include <stdexcept>

namespace chess {
void Game::generate_knight_moves(std::vector<Move>& moves, Square square) const {
  const Piece knight = this->get_piece(square);
  if (!knight.is_valid() or !knight.is_knight()) {
    throw std::runtime_error(std::format("Missing piece at square {}, expected a knight", square.to_string()));
  }

  const int directions[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  for (const auto& dir : directions) {
    const Square target_square(square.file + dir[0], square.rank + dir[1]);
    if (!target_square.is_valid()) {
      continue;
    }

    const Piece target_piece = this->get_piece(target_square);
    if (target_piece.is_empty()) {
      moves.push_back(Move(square, target_square, knight));
    } else {
      if (knight.is_opponent(target_piece)) {
        moves.push_back(Move(square, target_square, knight).set_captured(target_piece));
      }
    }
  }
}

}  // namespace chess
