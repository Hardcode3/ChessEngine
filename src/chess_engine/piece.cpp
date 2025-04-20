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

bool Piece::operator==(const Piece& other) const noexcept { return type == other.type && color == other.color; }

bool Piece::operator!=(const Piece& other) const noexcept { return !(*this == other); }
}  // namespace chess
