#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace chess {

struct Square {
  uint8_t file;  // 0-7 (a-h)
  uint8_t rank;  // 0-7 (1-8)

  constexpr Square(uint8_t f, uint8_t r) : file(f), rank(r) {}
  Square(const std::string &notation);  // e.g., "e4"

  constexpr bool is_valid() const { return file < 8 && rank < 8; }

  std::string to_string() const;

  bool operator==(const Square &other) const;
  bool operator!=(const Square &other) const;

};

}  // namespace chess