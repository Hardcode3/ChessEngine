#pragma once

#include <chess_engine/exceptions.hpp>
#include <string>

namespace chess {

struct Square {
  uint8_t file;  // 0-7 (a-h)
  uint8_t rank;  // 0-7 (1-8)
  static const std::string ND;

  explicit Square() = delete;
  explicit Square(uint8_t file, uint8_t rank, ThrowMode throw_mode = ThrowMode::NO_THROW);
  explicit Square(const std::string &notation);  // e.g., "e4"

  std::string to_string() const noexcept;

  static bool is_valid(const uint8_t &file, const uint8_t &rank);
  bool is_valid() const noexcept;
  bool is_file_valid() const noexcept;
  bool is_rank_valid() const noexcept;

  bool operator==(const Square &other) const;
  bool operator!=(const Square &other) const;
};

}  // namespace chess