#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <cstdint>

namespace Attacks {

constexpr uint64_t knight_attacks_for_square(int sq) {
  const uint64_t bb = 1ULL << sq;
  uint64_t attacks = 0ULL;

  using namespace Bitmasks;

  attacks |= (bb << 17) & ~FILE_A;             // NNE
  attacks |= (bb << 15) & ~FILE_H;             // NNW
  attacks |= (bb << 10) & ~(FILE_A | FILE_B);  // ENN
  attacks |= (bb << 6) & ~(FILE_G | FILE_H);   // WNN
  attacks |= (bb >> 17) & ~FILE_H;             // SSE
  attacks |= (bb >> 15) & ~FILE_A;             // SSW
  attacks |= (bb >> 10) & ~(FILE_G | FILE_H);  // ESS
  attacks |= (bb >> 6) & ~(FILE_A | FILE_B);   // WSS

  return attacks;
}

constexpr std::array<Bitboard, 64> KNIGHT_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(knight_attacks_for_square(sq));
  }
  return table;
}();

constexpr uint64_t king_attacks_for_square(int sq) {
  const uint64_t bb = 1ULL << sq;
  uint64_t attacks = 0ULL;

  using namespace Bitmasks;

  // Cardinal directions
  // No wrap around for N/S moves because the number falls off 64-bit integer
  // aka. <<8 on rank 8 make the N attack square fall off 64-bit integer
  // Hence it's 0, no horizontal wrap around
  attacks |= (bb << 8);            // North
  attacks |= (bb >> 8);            // South
  attacks |= (bb << 1) & ~FILE_A;  // East
  attacks |= (bb >> 1) & ~FILE_H;  // West

  // Diagonals
  attacks |= (bb << 9) & ~FILE_A;  // North-East
  attacks |= (bb << 7) & ~FILE_H;  // North-West
  attacks |= (bb >> 7) & ~FILE_A;  // South-East
  attacks |= (bb >> 9) & ~FILE_H;  // South-West

  return attacks;
}

constexpr std::array<Bitboard, 64> KING_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(king_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Attacks