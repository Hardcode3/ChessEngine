#pragma once
#include <chess_engine/square.hpp>
#include <cstdint>
#include <iostream>

/**
 * @class Bitboard
 * @brief Wrapper around a 64-bit unsigned integer to represent a chessboard.
 *
 * - Bit = 0 → empty square
 * - Bit = 1 → occupied square
 *
 * Representation (bit index mapping to board squares):
 *
 *   63 62 61 60 59 58 57 56    A8 B8 C8 D8 E8 F8 G8 H8
 *   55 54 53 52 51 50 49 48    A7 B7 C7 D7 E7 F7 G7 H7
 *   47 46 45 44 43 42 41 40    A6 B6 C6 D6 E6 F6 G6 H6
 *   39 38 37 36 35 34 33 32    A5 B5 C5 D5 E5 F5 G5 H5
 *   31 30 29 28 27 26 25 24    A4 B4 C4 D4 E4 F4 G4 H4
 *   23 22 21 20 19 18 17 16    A3 B3 C3 D3 E3 F3 G3 H3
 *   15 14 13 12 11 10  9  8    A2 B2 C2 D2 E2 F2 G2 H2
 *    7  6  5  4  3  2  1  0    A1 B1 C1 D1 E1 F1 G1 H1
 *
 * This layout makes pawn moves intuitive:
 * - White pawn push = shift north (+8 bits)
 * - Black pawn push = shift south (−8 bits)
 *
 */
class Bitboard {
 private:
  uint64_t m_bb;  ///< Raw 64-bit bitboard value

 public:
  /** @brief Constructs an empty bitboard (all bits = 0). */
  Bitboard();

  /** @brief Constructs a bitboard from a raw 64-bit value. */
  Bitboard(uint64_t value);

  /** @brief Returns the raw 64-bit value of the bitboard. */
  uint64_t value();

  /**
   * @brief Sets a bit (places a piece) on a given square.
   *
   * Internals:
   * - (1ULL << sq) shifts the value `1` left by `sq` positions.
   *   Example: if sq = 0 → mask = 000...0001 (A1)
   *            if sq = 4 → mask = 000...10000 (E1)
   * - 'bitwise or' |= sets that bit to `1` while leaving others unchanged.
   *
   * Example: set(E2) marks E2 as occupied.
   *
   * Start bitboard: 00000000 00000000 ... 00000000 00000000
   * 1ULL = 00000000 00000000 ... 00000000 00000001
   * 1ULL << E2 = 00000000 00000000 ... 00010000 00000000
   * m_bb |= (1ULL << sq) = 00000000 00000000 ... 00010000 00000000
   */
  void set(Square sq);
  void set(Square::Value sq);

  /**
   * @brief Clears a bit (removes a piece) on a given square.
   *
   * Internals:
   * - (1ULL << sq) creates a mask with a `1` only at position `sq`.
   * - ~(1ULL << sq) flips bits: all `1`s except position `sq` = `0`.
   * - 'bitwise and' &= with this mask forces that square to `0` (cleared).
   *
   * Example: clear(E2) empties square E2.
   *
   * Start bitboard: 00000000 00000000 ... 00010000 00000000
   * 1ULL = 00000000 00000000 ... 00000000 00000001
   * 1ULL << E2 = 00000000 00000000 ... 00010000 00000000
   * ~(1ULL << E2) = 11111111 11111111 ... 11101111 11111111
   * m_bb &= ~(1ULL << sq) = 00000000 00000000 ... 00000000 00000000
   */
  void clear(Square sq);
  void clear(Square::Value sq);

  /**
   * @brief Checks if a square is occupied.
   *
   * Internals:
   * - (m_bb >> sq) shifts the bitboard right so that the target bit
   *   moves into the least significant position.
   * - & 1ULL masks out all other bits except the least significant one.
   *
   * Example: test(E2) → returns true if E2 bit = 1.
   *
   * Start bitboard: 00000000 00000000 ... 00010000 00000000
   * 1ULL = 00000000 00000000 ... 0000000 00000001
   * m_bb >> E2 = 00000000 00000000 ... 0000000 00000001
   * (m_bb >> E2) & 1ULL = true
   *
   */
  bool test(Square sq) const;
  bool test(Square::Value sq) const;

  /** @brief Clears the whole bitboard (all bits = 0). */
  void reset();

  /**
   * @brief Prints the bitboard as an 8×8 grid.
   *
   * - "1" means the square is occupied
   * - "." means the square is empty
   *
   * The output starts from rank 8 down to rank 1.
   */
  void print() const;

  bool operator==(const Bitboard& other) const;
  bool operator!=(const Bitboard& other) const;
  Bitboard operator|(const Bitboard& other) const;
  Bitboard operator&(const Bitboard& other) const;
  Bitboard operator~() const;
  Bitboard& operator|=(const Bitboard& other);
  Bitboard& operator&=(const Bitboard& other);
};
