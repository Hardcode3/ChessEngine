#include <fmt/core.h>

#include <chess_engine/piece.hpp>

Piece::Piece(char c) {
  m_symbol = c;
  switch (c) {
    case 'P':
      m_type = P;
      break;
    case 'N':
      m_type = N;
      break;
    case 'B':
      m_type = B;
      break;
    case 'R':
      m_type = R;
      break;
    case 'Q':
      m_type = Q;
      break;
    case 'K':
      m_type = K;
      break;
    case 'p':
      m_type = p;
      break;
    case 'n':
      m_type = n;
      break;
    case 'b':
      m_type = b;
      break;
    case 'r':
      m_type = r;
      break;
    case 'q':
      m_type = q;
      break;
    case 'k':
      m_type = k;
      break;
    case '.':
      m_type = NO_PIECE;
      break;
    default:
      const std::string msg = fmt::format("Invalid piece character {}", c);
      throw std::invalid_argument(msg);
  }
}

char Piece::to_char() const { return m_symbol; }
