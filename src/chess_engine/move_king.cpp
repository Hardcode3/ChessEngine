#include <chess_engine/game.hpp>
#include <format>
#include <stdexcept>

namespace chess {
void Game::generate_king_moves(std::vector<Move>& moves, Square square) const {
  const Piece king = this->get_piece(square);
  if (!king.is_valid() or !king.is_king()) {
    throw std::runtime_error(std::format("Missing piece at square {}, expected a king", square.to_string()));
  }

  const int directions[8][2] = {{1, 1}, {0, 1}, {-1, 1}, {1, -1}, {0, -1}, {-1, -1}, {1, 0}, {-1, 0}};

  for (const auto& dir : directions) {
    Square target_square(square.file + dir[0], square.rank + dir[1]);

    if (!target_square.is_valid()) continue;

    const Piece target_piece = this->get_piece(target_square);

    if (target_piece.is_empty()) {
      // No enemy piece encountered, add the move
      moves.push_back(Move(square, target_square, king));
      continue;
    } else if (king.is_opponent(target_piece)) {
      // Enemy piece encountered, add the capture move
      moves.push_back(Move(square, target_square, king).set_captured(target_piece));
      continue;
    } else {
      // Own piece encountered
      continue;
    }
  }
}

}  // namespace chess
