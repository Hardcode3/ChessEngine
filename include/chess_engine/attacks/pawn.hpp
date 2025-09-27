#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <cstdint>

namespace Attacks {

// Single pushes (1 square forward)
constexpr std::array<Bitboard, 64> WHITE_PAWN_SINGLE_PUSH = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    // ranks 1..7 can push north (+8) for white pawns
    if (sq / 8 < 7) {
      table[sq] = Bitboard(1ULL << (sq + 8));
    }
  }
  return table;
}();

constexpr std::array<Bitboard, 64> BLACK_PAWN_SINGLE_PUSH = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    // ranks 2..8 can push south for black pawns
    if (sq / 8 > 0) {
      table[sq] = Bitboard(1ULL << (sq - 8));
    }
  }
  return table;
}();

// Double pushes from starting rank
constexpr std::array<Bitboard, 64> WHITE_PAWN_DOUBLE_PUSH = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    // white can only double push from rank 2
    if (sq / 8 == 1) {
      table[sq] = Bitboard(1ULL << (sq + 16));
    }
  }
  return table;
}();

constexpr std::array<Bitboard, 64> BLACK_PAWN_DOUBLE_PUSH = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    // black can only double push from rank 7
    if (sq / 8 == 6) {
      table[sq] = Bitboard(1ULL << (sq - 16));
    }
  }
  return table;
}();

// Pawn attacks (diagonal captures)
constexpr std::array<Bitboard, 64> WHITE_PAWN_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};

  using namespace Bitmasks;

  for (int sq = 0; sq < 64; ++sq) {
    uint64_t bb = 1ULL << sq;
    uint64_t attacks = 0;
    if (sq / 8 < 7) {        // rank 1..7
      attacks |= (bb << 7);  // NW
      attacks |= (bb << 9);  // NE
    }
    table[sq] = Bitboard(attacks);
  }
  return table;
}();

constexpr std::array<Bitboard, 64> BLACK_PAWN_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};

  using namespace Bitmasks;

  for (int sq = 0; sq < 64; ++sq) {
    uint64_t bb = 1ULL << sq;
    uint64_t attacks = 0;
    if (sq / 8 > 0) {        // rank 2..8
      attacks |= (bb >> 9);  // SW
      attacks |= (bb >> 7);  // SE
    }
    table[sq] = Bitboard(attacks);
  }
  return table;
}();

}  // namespace Attacks