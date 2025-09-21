#pragma once
#include <chess_engine/square.hpp>
#include <cstdint>
#include <iostream>

/**
 * @class Bitboard
 * @brief Wrapper around a 64-bit unsigned integer to represent a bitboard.
 *
 * - Bit = 0 -> empty square
 * - Bit = 1 -> occupied square
 *
 *   Bit indices (uint64_t bits):        Chessboard squares:
 *
 *   63 62 61 60 59 58 57 56             H8 G8 F8 E8 D8 C8 B8 A8
 *   55 54 53 52 51 50 49 48             H7 G7 F7 E7 D7 C7 B7 A7
 *   47 46 45 44 43 42 41 40             H6 G6 F6 E6 D6 C6 B6 A6
 *   39 38 37 36 35 34 33 32             H5 G5 F5 E5 D5 C5 B5 A5
 *   31 30 29 28 27 26 25 24             H4 G4 F4 E4 D4 C4 B4 A4
 *   23 22 21 20 19 18 17 16             H3 G3 F3 E3 D3 C3 B3 A3
 *   15 14 13 12 11 10  9  8             H2 G2 F2 E2 D2 C2 B2 A2
 *    7  6  5  4  3  2  1  0             H1 G1 F1 E1 D1 C1 B1 A1
 *
 * Notes:
 * - Bit 0 = A1 (least significant bit).
 * - Bit 63 = H8 (most significant bit).
 * - Each rank = 8 consecutive bits.
 *
 * This layout makes pawn moves intuitive:
 * - White pawn push = shift north (+8 bits)
 * - Black pawn push = shift south (−8 bits)
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
