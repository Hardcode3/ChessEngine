#include <chess_engine/square.hpp>
#include <format>

namespace chess {

Square::Square(uint8_t file, uint8_t rank) : file(file), rank(rank) {
  if (file > 7 || rank > 7) {
    throw std::invalid_argument(std::format("Square out of bounds, invalid file {}, rank {}", file, rank));
  }
}

Square::Square(const std::string &notation) {
  if (notation.length() != 2) {
    throw std::invalid_argument(std::format("Invalid notation {}, expected two characters ([a-g][1-8])", notation));
  }
  // chars are ASCII, so e - a = 101 - 97 = 4
  file = notation[0] - 'a';
  rank = notation[1] - '1';
  if (file > 7 || rank > 7) {
    throw std::invalid_argument(
        std::format("Square out of bounds, invalid notation {} would be file {}, rank {}", notation, file, rank));
  }
}

std::string Square::to_string() const {
  if (file > 7 || rank > 7) {
    throw std::invalid_argument(
        std::format("Invalid position for file {}, rank {} cannot be converted to notation", file, rank));
  }
  return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

bool Square::operator==(const Square &other) const { return this->file == other.file && this->rank == other.rank; }

bool Square::operator!=(const Square &other) const { return !(*this == other); }

}  // namespace chess