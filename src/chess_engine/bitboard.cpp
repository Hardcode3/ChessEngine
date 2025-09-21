#include <chess_engine/bitboard.hpp>

Bitboard::Bitboard() : m_bb() {}  // Zero initialization of uint64_t m_bb granted by C++ standard

Bitboard::Bitboard(uint64_t value) : m_bb(value) {}

uint64_t Bitboard::value() { return m_bb; }

void Bitboard::set(Square sq) { m_bb |= (1ULL << sq.value()); }
void Bitboard::set(Square::Value sq) { m_bb |= (1ULL << sq); }

void Bitboard::clear(Square sq) { m_bb &= ~(1ULL << sq.value()); }
void Bitboard::clear(Square::Value sq) { m_bb &= ~(1ULL << sq); }

bool Bitboard::test(Square sq) const {
  Square::Value v = sq.value();
  int vi = static_cast<int>(v);
  uint64_t val = (m_bb >> sq.value());
  bool res = val & 1ULL;

  return res;
}
bool Bitboard::test(Square::Value sq) const { return (m_bb >> sq) & 1ULL; }

void Bitboard::reset() { m_bb = 0ULL; }

void Bitboard::print() const {
  for (int rank = 7; rank >= 0; --rank) {
    for (int file = 0; file < 8; ++file) {
      const Square sq(file, rank);
      std::cout << (test(sq) ? "1 " : ". ");
    }
    std::cout << "\n";
  }
}

bool Bitboard::operator==(const Bitboard& other) const { return m_bb == other.m_bb; }
bool Bitboard::operator!=(const Bitboard& other) const { return m_bb != other.m_bb; }

Bitboard Bitboard::operator|(const Bitboard& other) const { return m_bb | other.m_bb; }
Bitboard Bitboard::operator&(const Bitboard& other) const { return m_bb & other.m_bb; }
Bitboard Bitboard::operator~() const { return ~m_bb; }

Bitboard& Bitboard::operator|=(const Bitboard& other) {
  m_bb |= other.m_bb;
  return *this;
}
Bitboard& Bitboard::operator&=(const Bitboard& other) {
  m_bb &= other.m_bb;
  return *this;
}
