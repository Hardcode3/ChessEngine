#include <cctype>
#include <chess_engine/piece.hpp>

namespace chess {
Piece::Piece(char c) {
  if (c == '.') {  // Empty square
    type = Type::EMPTY;
    color = Color::NO_COLOR;
  } else {
    // Check color
    if (std::islower(c)) {
      color = Color::BLACK;
    } else {
      color = Color::WHITE;
    }

    // Map char to piece type
    switch (std::tolower(c)) {
      case 'p':
        type = Type::PAWN;
        break;
      case 'n':
        type = Type::KNIGHT;
        break;
      case 'b':
        type = Type::BISHOP;
        break;
      case 'r':
        type = Type::ROOK;
        break;
      case 'q':
        type = Type::QUEEN;
        break;
      case 'k':
        type = Type::KING;
        break;
      default:
        type = Type::EMPTY;
        color = Color::NO_COLOR;
        break;
    }
  }
}

char Piece::to_char() const {
  char type_char;
  switch (type) {
    case Type::PAWN:
      type_char = 'p';
      break;
    case Type::KNIGHT:
      type_char = 'n';
      break;
    case Type::BISHOP:
      type_char = 'b';
      break;
    case Type::ROOK:
      type_char = 'r';
      break;
    case Type::QUEEN:
      type_char = 'q';
      break;
    case Type::KING:
      type_char = 'k';
      break;
    case Type::EMPTY:
      type_char = '.';
      break;
  }

  if (color == Color::WHITE) {
    return std::toupper(type_char);
  }
  return type_char;
}

int8_t Piece::get_direction() const noexcept {
  if (!is_valid()) {
    return std::numeric_limits<int8_t>::max();
  }
  return (color == Color::WHITE) ? 1 : -1;
}

uint8_t Piece::get_starting_rank() const noexcept {
  if (!is_valid()) {
    return std::numeric_limits<uint8_t>::max();
  }

  if (type == Type::PAWN) {
    return (color == Color::WHITE) ? 1 : 6;
  } else {
    return (color == Color::WHITE) ? 0 : 7;
  }
}

uint8_t Piece::get_promotion_rank() const noexcept {
  if (!is_valid()) {
    return std::numeric_limits<uint8_t>::max();
  }
  return (color == Color::WHITE) ? 7 : 0;
}

bool Piece::is_opponent(const Piece& other) const noexcept {
  if (this->has_color() and other.has_color()) {
    return this->color != other.color;
  }
  return false;
}

bool Piece::operator==(const Piece& other) const noexcept { return type == other.type && color == other.color; }

bool Piece::operator!=(const Piece& other) const noexcept { return !(*this == other); }
}  // namespace chess
