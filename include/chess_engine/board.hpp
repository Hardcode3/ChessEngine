#pragma once

#include <array>
#include <chess_engine/color.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace chess {

class Board {
 private:
  std::array<std::array<Piece, 8>, 8> pieces;
  std::array<std::array<Color, 8>, 8> colors;

  Color side_to_move;           // WHITE moves first, then players alternate
  bool white_castle_kingside;   // Can White castle kingside (O-O)? Requires: king
                                // and rook haven't moved, no pieces between them
  bool white_castle_queenside;  // Can White castle queenside (O-O-O)? Same
                                // conditions as kingside
  bool black_castle_kingside;   // Can Black castle kingside? Same conditions
  bool black_castle_queenside;  // Can Black castle queenside? Same conditions
  Square en_passant_square;     // Square where en passant capture is possible.
                                // Occurs when pawn moves 2 squares forward
  int halfmove_clock;           // Number of half-moves since last capture or pawn move
                                // (for 50-move rule)
  int fullmove_number;          // Current move number, increments after Black's move

  void generate_pawn_moves(std::vector<Move> &moves, Square square) const;
  void generate_knight_moves(std::vector<Move> &moves, Square square) const;
  void generate_bishop_moves(std::vector<Move> &moves, Square square) const;
  void generate_rook_moves(std::vector<Move> &moves, Square square) const;
  void generate_queen_moves(std::vector<Move> &moves, Square square) const;
  void generate_king_moves(std::vector<Move> &moves, Square square) const;

  bool is_square_attacked(Square square, Color attacker) const;
  bool is_in_check(Color color) const;
  bool is_move_legal(const Move &move) const;
  Square get_king_square(Color color) const;
  bool is_square_safe(Square square, Color color) const;

 public:
  static constexpr const char *STARTING_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Board();

  void load_fen(const std::string &fen);
  std::string get_fen() const;

  Move uci_to_move(const std::string &uci_move) const;
  std::string move_to_uci(const Move &move) const;

  std::vector<Move> generate_legal_moves() const;

  void make_move(Move &move);
  void unmake_move(const Move &move);

  int evaluate() const;

  Piece get_piece(Square square) const { return pieces[square.rank][square.file]; }
  Color get_color(Square square) const { return colors[square.rank][square.file]; }
  Color get_side_to_move() const { return side_to_move; }
  bool is_game_over() const;
};

}  // namespace chess