#pragma once
#include <cctype>
#include <stdexcept>

/**
 * @brief Represents a chess piece.
 *
 * This class wraps a lightweight enum and provides utility methods
 * for querying piece color, converting to/from characters, and comparison.
 *
 * By convention:
 * - White pieces are uppercase (P, N, B, R, Q, K)
 * - Black pieces are lowercase (p, n, b, r, q, k)
 * - NO_PIECE represents an empty square
 */
class Piece {
 public:
  /** @brief Enum for piece types */
  enum Type : int { P, N, B, R, Q, K, p, n, b, r, q, k, NO_PIECE };

 private:
  Type m_type;
  char m_symbol;

 public:
  /**
   * @brief Constructs a piece.
   * @param c Type of the piece to build
   * @throw std::invalid_argument when the Piece character is invalid
   */
  Piece(char c = '.');

  /**
   * @brief Returns the underlying enum type of the piece.
   * @return Type of the piece
   */
  Type type() const { return m_type; }

  /**
   * @brief Checks if the piece is white.
   * @return true if the piece is white, false otherwise
   */
  bool is_white() const { return m_type >= P && m_type <= K; }

  /**
   * @brief Checks if the piece is black.
   * @return true if the piece is black, false otherwise
   */
  bool is_black() const { return m_type >= p && m_type <= k; }

  /**
   * @brief Checks if the piece represents no piece.
   * @return true if NO_PIECE, false otherwise
   */
  bool is_none() const { return m_type == NO_PIECE; }

  /**
   * @brief Converts the piece to a printable character.
   * @return 'P','N',...,'k' for pieces, '.' for NO_PIECE
   */
  char to_char() const;

  bool operator==(const Piece& other) const { return m_type == other.m_type; }
  bool operator!=(const Piece& other) const { return m_type != other.m_type; }
};