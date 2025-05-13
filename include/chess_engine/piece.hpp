#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace chess {

class Piece {
 public:
  enum class Type : uint8_t { EMPTY = 0, PAWN = 1, KNIGHT = 2, BISHOP = 3, ROOK = 4, QUEEN = 5, KING = 6 };
  enum class Color : uint8_t { NO_COLOR = 0, WHITE = 1, BLACK = 2 };

  Type type;
  Color color;

  Piece(Type t = Type::EMPTY, Color c = Color::NO_COLOR) : type(t), color(c) {}
  Piece(char c);

  char to_char() const;
  bool is_valid() const noexcept { return type != Type::EMPTY && color != Color::NO_COLOR; }
  bool is_empty() const noexcept { return type == Type::EMPTY; }
  bool is_populated() const noexcept { return !is_empty(); }
  bool has_color() const noexcept { return color != Color::NO_COLOR; }
  int8_t get_direction() const noexcept;
  uint8_t get_starting_rank() const noexcept;
  uint8_t get_promotion_rank() const noexcept;

  bool is_pawn() const noexcept { return type == Type::PAWN; }
  bool is_knight() const noexcept { return type == Type::KNIGHT; }
  bool is_rook() const noexcept { return type == Type::ROOK; }
  bool is_bishop() const noexcept { return type == Type::BISHOP; }

  bool is_opponent(const Piece& other) const noexcept;

  bool operator==(const Piece& other) const noexcept;
  bool operator!=(const Piece& other) const noexcept;
};

}  // namespace chess