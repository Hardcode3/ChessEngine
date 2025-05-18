#include <chess_engine/game.hpp>
#include <format>
#include <stdexcept>

namespace chess {
void Game::generate_bishop_moves(std::vector<Move>& moves, Square square) const {
  const Piece bishop = this->get_piece(square);
  if (!bishop.is_valid() or !bishop.is_bishop()) {
    throw std::runtime_error(std::format("Missing piece at square {}, expected a bishop", square.to_string()));
  }

  const int directions[4][2] = {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

  for (const auto& dir : directions) {
    Square target_square(square.file + dir[0], square.rank + dir[1]);

    while (target_square.is_valid()) {
      const Piece target_piece = this->get_piece(target_square);
      if (target_piece.is_empty()) {
        // No enemy piece encountered, add the move
        moves.push_back(Move(square, target_square, bishop));
      } else if (bishop.is_opponent(target_piece)) {
        // Enemy piece encountered, add the capture move
        // And stop search in this direction
        moves.push_back(Move(square, target_square, bishop).set_captured(target_piece));
        break;
      } else {
        // Own piece encountered, stop the search in this direction
        break;
      }

      // Update square coordinates for next iteration
      target_square.file += dir[0];
      target_square.rank += dir[1];
    }
  }
}

}  // namespace chess
