#include <chess_engine/move.hpp>
#include <format>

namespace chess {

CastlingInfo::CastlingInfo(Square from, Square to) : rook_from(from), rook_to(to) {
  if (rook_from == rook_to) {
    throw std::invalid_argument("From and to squares cannot be the same");
  }
}

PromotionInfo::PromotionInfo(Piece piece) : promoted_to(piece) {
  if (!piece.is_valid()) {
    throw std::invalid_argument("Promotion piece must be valid");
  }
}

Move::Move(Square from, Square to, Piece piece) : from(from), to(to), piece(piece) {
  if (!piece.is_valid()) {
    throw std::invalid_argument("Move piece must be valid");
  }
}

// Getters
Square Move::get_from() const noexcept { return from; }
Square Move::get_to() const noexcept { return to; }
Piece Move::get_piece() const noexcept { return piece; }
const std::optional<Piece>& Move::get_captured() const noexcept { return captured; }
const std::optional<CastlingInfo>& Move::get_castling() const noexcept { return castling; }
const std::optional<PromotionInfo>& Move::get_promotion() const noexcept { return promotion; }
const std::optional<GameState>& Move::get_previous_state() const noexcept { return previous_state; }

// Convenience getters
Piece Move::get_promoted_to() const noexcept { return is_promotion() ? promotion->promoted_to : Piece(); }

// Setters with validation
void Move::set_captured(Piece c) noexcept {
  captured = c;  // Allow setting any piece, including empty ones
}

void Move::set_castling(Square rook_from, Square rook_to) noexcept {
  if (rook_from != rook_to) {  // Validate rook movement
    castling = CastlingInfo(rook_from, rook_to);
  }
}

void Move::set_promotion(Piece promoted_to) noexcept {
  if (promoted_to.is_valid()) {  // Only set if valid
    promotion = PromotionInfo(promoted_to);
  }
}

void Move::set_previous_state(GameState state) { previous_state = std::move(state); }

// Queries
bool Move::is_capture() const noexcept { return captured.has_value(); }
bool Move::is_promotion() const noexcept { return promotion.has_value(); }
bool Move::is_castling() const noexcept { return castling.has_value(); }
bool Move::is_quiet() const noexcept { return !is_capture() && !is_promotion() && !is_castling(); }

// Additional descriptive queries
bool Move::is_pawn_move() const noexcept { return piece.type == Piece::Type::PAWN; }
bool Move::is_pawn_double_push() const noexcept {
  return is_pawn_move() && std::abs(static_cast<int>(from.rank) - static_cast<int>(to.rank)) == 2;
}
bool Move::is_en_passant() const noexcept {
  return is_pawn_move() && is_capture() && from.file != to.file && (!captured.has_value() || !captured->is_valid());
}
bool Move::is_king_side_castle() const noexcept { return is_castling() && to.file > from.file; }
bool Move::is_queen_side_castle() const noexcept { return is_castling() && to.file < from.file; }

std::string Move::to_uci() const {
  std::string result;
  // convert back to char from int (using the offset of the ASCII table)
  result += static_cast<char>('a' + from.file);
  result += static_cast<char>('1' + from.rank);
  result += static_cast<char>('a' + to.file);
  result += static_cast<char>('1' + to.rank);
  if (is_promotion()) {
    result += promotion->promoted_to.to_char();
  }
  return result;
}

}  // namespace chess