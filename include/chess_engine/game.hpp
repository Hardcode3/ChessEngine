#pragma once

#include <chess_engine/board.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/state.hpp>
#include <string>
#include <vector>

namespace chess {

class Game {
 public:
  Game();

  // Game state management
  void from_fen(const std::string& fen);
  std::string get_fen() const;
  bool is_game_over() const;
  Piece::Color get_side_to_move() const { return state.side_to_move; }

  // Move generation and validation
  std::vector<Move> generate_legal_moves() const;
  bool is_move_legal(const Move& move) const;
  void make_move(const Move& move);
  void unmake_move(const Move& move);

  // Move conversion
  Move uci_to_move(const std::string& uci_move) const;
  std::string move_to_uci(const Move& move) const;

  // Board access
  const Board& get_board() const { return board; }
  Piece get_piece(Square square) const { return board.get_piece(square); }

  // Game state queries
  const GameState& get_state() const { return state; }
  bool is_in_check(Piece::Color color) const;
  bool is_square_attacked(Square square, Piece::Color attacker) const;
  Square get_king_square(Piece::Color color) const;

 private:
  Board board;
  GameState state;
  std::vector<Move> move_history;

  // Move generation helpers
  void generate_pawn_moves(std::vector<Move>& moves, Square square) const;
  void generate_knight_moves(std::vector<Move>& moves, Square square) const;
  void generate_bishop_moves(std::vector<Move>& moves, Square square) const;
  void generate_rook_moves(std::vector<Move>& moves, Square square) const;
  void generate_queen_moves(std::vector<Move>& moves, Square square) const;
  void generate_king_moves(std::vector<Move>& moves, Square square) const;

  // Move validation helpers
  bool is_square_safe(Square square, Piece::Color color) const;
  bool is_castling_legal(const Move& move) const;
  bool is_en_passant_legal(const Move& move) const;
};

}  // namespace chess