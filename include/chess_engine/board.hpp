#pragma once
#include <chess_engine/bitboard.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

class Board {
 private:
  Bitboard m_w_pawns, m_w_rooks, m_w_bishops, m_w_knights, m_w_king, m_w_queen;
  Bitboard m_b_pawns, m_b_rooks, m_b_bishops, m_b_knights, m_b_king, m_b_queen;

  // Game state
  bool m_is_white_turn;
  std::optional<Square> m_en_passant_sq;

  // Castling abilies
  bool m_white_castle_kingside, m_white_castle_queenside;
  bool m_black_castle_kingside, m_black_castle_queenside;

  // 50 move rule
  int m_halfmove_clock;

  // Starts at 1, increments after Black’s move
  int m_fullmove_number;

 public:
  Board();
  Board(const std::string& fen);
  Bitboard white_pieces() const;
  Bitboard black_pieces() const;
  Bitboard occupied() const;
  Piece get_piece(Square sq) const;
  void set_piece(Square sq, Piece p);
  void remove_piece(Square sq);
  void print(std::ostream& os) const;
};
