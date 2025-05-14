#include <chess_engine/square.hpp>
#include <format>
#include <limits>
#include <type_traits>

namespace chess {

const std::string Square::ND = "ND";

Square::Square() : file(std::numeric_limits<uint8_t>::max()), rank(std::numeric_limits<uint8_t>::max()) {}

Square::Square(uint8_t file, uint8_t rank, ThrowMode throw_mode) : file(file), rank(rank) {
  if (throw_mode == ThrowMode::THROW && !Square::is_valid(file, rank)) {
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
  if (!is_valid()) {
    throw std::invalid_argument(
        std::format("Square out of bounds, invalid notation {} would be file {}, rank {}", notation, file, rank));
  }
}

std::string Square::to_string() const noexcept {
  if (!is_valid()) {
    return Square::ND;
  }
  return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

bool Square::is_valid(const uint8_t &file, const uint8_t &rank) {
  return (file >= 0 && file < 8 && rank >= 0 && rank < 8);
}

bool Square::is_valid() const noexcept { return (is_file_valid() && is_rank_valid()); }

bool Square::is_file_valid() const noexcept { return (file >= 0 && file < 8); }

bool Square::is_rank_valid() const noexcept { return (rank >= 0 && rank < 8); }

bool Square::operator==(const Square &other) const { return this->file == other.file && this->rank == other.rank; }

bool Square::operator!=(const Square &other) const { return !(*this == other); }

}  // namespace chess