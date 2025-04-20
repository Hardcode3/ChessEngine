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
};

}  // namespace chess