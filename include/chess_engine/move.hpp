#pragma once

#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <chess_engine/state.hpp>
#include <optional>
#include <string>

namespace chess {

struct GameState;

struct CastlingInfo {
  Square rook_from;
  Square rook_to;

  CastlingInfo(Square from, Square to);
};

struct PromotionInfo {
  Piece promoted_to;

  PromotionInfo(Piece piece);
};

class Move {
 public:
  Move(Square from, Square to, Piece piece);

  // Getters
  Square get_from() const noexcept;
  Square get_to() const noexcept;
  Piece get_piece() const noexcept;
  const std::optional<Piece>& get_captured() const noexcept;
  const std::optional<CastlingInfo>& get_castling() const noexcept;
  const std::optional<PromotionInfo>& get_promotion() const noexcept;
  const std::optional<GameState>& get_previous_state() const noexcept;

  // Convenience getters
  Piece get_promoted_to() const noexcept;

  // Setters with validation
  void set_captured(Piece c) noexcept;
  void set_castling(Square rook_from, Square rook_to) noexcept;
  void set_promotion(Piece promoted_to) noexcept;
  void set_previous_state(GameState state);

  // Queries
  bool is_capture() const noexcept;
  bool is_promotion() const noexcept;
  bool is_castling() const noexcept;
  bool is_quiet() const noexcept;

  // Additional descriptive queries
  bool is_pawn_move() const noexcept;
  bool is_pawn_double_push() const noexcept;
  bool is_en_passant() const noexcept;
  bool is_king_side_castle() const noexcept;
  bool is_queen_side_castle() const noexcept;

  std::string to_uci() const;

 private:
  Square from;
  Square to;
  Piece piece;
  std::optional<Piece> captured;
  std::optional<CastlingInfo> castling;
  std::optional<PromotionInfo> promotion;
  std::optional<GameState> previous_state;
};

}  // namespace chess