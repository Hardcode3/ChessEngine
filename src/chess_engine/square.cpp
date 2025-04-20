#include <algorithm>
#include <chess_engine/square.hpp>
#include <sstream>
#include <stdexcept>

namespace chess {

Square::Square(const std::string &notation) {
  if (notation.length() != 2) {
    throw std::invalid_argument("Invalid square notation");
  }
  // chars are ASCII, so e - a = 101 - 97 = 4
  file = notation[0] - 'a';
  rank = notation[1] - '1';
  if (file > 7 || rank > 7) {
    throw std::invalid_argument("Square out of bounds");
  }
}

std::string Square::to_string() const {
  if (!this->is_valid()) {
    return "??";
  }
  return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

bool Square::operator==(const Square &other) const { return this->file == other.file && this->rank == other.rank; }

bool Square::operator!=(const Square &other) const { return !(*this == other); }

}  // namespace chess