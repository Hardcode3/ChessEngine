#include <chess_engine/game.hpp>
#include <format>
#include <stdexcept>

namespace chess {

void Game::generate_pawn_moves(std::vector<Move>& moves, Square square) const {
  const Piece pawn = this->board.get_piece(square);
  if (!pawn.is_valid()) {
    throw std::runtime_error(std::format("Missing piece at square {}, expected a pawn", square.to_string()));
  }

  const int8_t direction = pawn.get_direction();
  const uint8_t starting_rank = pawn.get_starting_rank();
  const uint8_t promotion_rank = pawn.get_promotion_rank();

  // Forward moves
  const Square one_forward(square.file, square.rank + direction);
  if (one_forward.is_rank_valid()) {
    const Piece one_forward_target = this->get_piece(one_forward);

    if (one_forward_target.is_empty()) {
      // Check for promotion
      if (one_forward.rank == promotion_rank) {
        moves.push_back(Move(square, one_forward, pawn).set_promotion(Piece(Piece::Type::QUEEN, pawn.color)));
        moves.push_back(Move(square, one_forward, pawn).set_promotion(Piece(Piece::Type::ROOK, pawn.color)));
        moves.push_back(Move(square, one_forward, pawn).set_promotion(Piece(Piece::Type::BISHOP, pawn.color)));
        moves.push_back(Move(square, one_forward, pawn).set_promotion(Piece(Piece::Type::KNIGHT, pawn.color)));
      } else {
        moves.push_back(Move(square, one_forward, pawn));
      }
    }

    // Two squares forward from starting position
    const Square two_forward(square.file, square.rank + 2 * direction);
    if (square.rank == starting_rank && two_forward.is_rank_valid()) {
      const Piece two_forward_target = this->get_piece(two_forward);
      if (one_forward_target.is_empty() && two_forward_target.is_empty()) {
        moves.push_back(Move(square, two_forward, pawn));
      }
    }
  }

  // Diagonal captures
  for (int8_t file_offset : {-1, 1}) {
    const int8_t target_file = square.file + file_offset;
    const int8_t target_rank = square.rank + direction;
    const Square diagonal_square(static_cast<uint8_t>(target_file), static_cast<uint8_t>(target_rank));
    if (diagonal_square.is_valid()) {
      const Piece diagonal_target = this->get_piece(diagonal_square);

      // Regular capture
      const bool is_populated = diagonal_target.is_populated();
      const bool is_opponent = pawn.is_opponent(diagonal_target);
      if (is_populated && is_opponent) {
        if (diagonal_square.rank == promotion_rank) {
          moves.push_back(Move(square, diagonal_square, pawn)
                              .set_promotion(Piece(Piece::Type::QUEEN, pawn.color))
                              .set_captured(diagonal_target));
          moves.push_back(Move(square, diagonal_square, pawn)
                              .set_promotion(Piece(Piece::Type::ROOK, pawn.color))
                              .set_captured(diagonal_target));
          moves.push_back(Move(square, diagonal_square, pawn)
                              .set_promotion(Piece(Piece::Type::BISHOP, pawn.color))
                              .set_captured(diagonal_target));
          moves.push_back(Move(square, diagonal_square, pawn)
                              .set_promotion(Piece(Piece::Type::KNIGHT, pawn.color))
                              .set_captured(diagonal_target));
        } else {
          moves.push_back(Move(square, diagonal_square, pawn).set_captured(diagonal_target));
        }
      }
      // En passant capture
      const bool en_passant_is_possible = this->state.en_passant_square.has_value();
      const bool target_is_en_passant = diagonal_square == this->state.en_passant_square.value_or(Square());
      if ( en_passant_is_possible && target_is_en_passant) {
        const Square en_passant_target_square(square.file + file_offset, square.rank);
        const Piece en_passant_target = board.get_piece(en_passant_target_square);
        moves.push_back(Move(square, diagonal_square, pawn).set_captured(en_passant_target));
      }
    }
  }
}
}  // namespace chess
