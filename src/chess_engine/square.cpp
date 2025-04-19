#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <chess_engine/square.hpp>

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

} // namespace chess