#pragma once

#include <string>

namespace chess {

struct Square {
  uint8_t file;  // 0-7 (a-h)
  uint8_t rank;  // 0-7 (1-8)

  Square() = delete;
  Square(uint8_t file, uint8_t rank);
  Square(const std::string &notation);  // e.g., "e4"

  std::string to_string() const;

  bool operator==(const Square &other) const;
  bool operator!=(const Square &other) const;
};

}  // namespace chess