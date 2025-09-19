#include <fmt/core.h>

#include <chess_engine/square.hpp>
#include <stdexcept>

Square::Square(Value v) : m_value(v) {}

Square::Square(int file, int rank) {
  // Rank refers to the eight horizontal rows on the board, labelled 1 to 8.
  // File refers to the eight vertical columns on the board, labelled a to h.
  if (file < 0 || file > 7 || rank < 0 || rank > 7) {
    const std::string msg = fmt::format("Invalid file ({}) or rank({}), must stay within ranke [0,7].", file, rank);
    throw std::invalid_argument(msg);
  } else {
    m_value = static_cast<Value>(rank * 8 + file);
  }
}

Square::Square(const std::string& s) {
  if (s.size() != 2) {
    const std::string msg = fmt::format("Invalid Square string '{}', must be built out of two characters", s);
    throw std::invalid_argument(msg);
  }

  const char file = std::tolower(s.at(0));
  const char rank = s.at(1);
  if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
    const std::string msg = fmt::format(
        "Invalid Square string '{}', must be built out of two characters, "
        "the first being the file (between a and h, got {}) and the second the rank (between 1 and 8, got {})",
        s, file, rank);
    throw std::invalid_argument(msg);
  }
  const int ifile = file - 'a';
  const int irank = rank - '1';
  m_value = static_cast<Value>(irank * 8 + ifile);
}

int Square::file() const { return static_cast<int>(m_value) % 8; }
int Square::rank() const { return static_cast<int>(m_value) / 8; }

std::string Square::to_string() const { return {char('a' + file()), char('1' + rank())}; }

bool Square::operator==(const Square& other) const { return m_value == other.m_value; }
bool Square::operator!=(const Square& other) const { return m_value != other.m_value; }