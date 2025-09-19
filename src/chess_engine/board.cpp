#include <chess_engine/board.hpp>
#include <sstream>

Board::Board() {}

Board::Board(const std::string& fen) {
  /*
   * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
   * FEN (Forsyth-Edwards Notation) has 6 fields separated by slashes and spaces:
   *
   * 1. Piece placement (ranks 8 → 1, separated by /).
   * 2. Side to move (w or b).
   * 3. Castling availability (KQkq or -).
   * 4. En passant target square (like e3 or -).
   * 5. Halfmove clock (for 50-move rule).
   * 6. Fullmove number (starts at 1).
   *
   * Example FEN for starting position: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
   */
  std::istringstream iss(fen);
  std::string token;
  iss >> token;

  // TODO

  // Second token: side to move
  iss >> token;
  m_is_white_turn = (token == "w") ? true : false;

  // Third token: Castling Rights
  iss >> token;
  m_white_castle_kingside = token.find('K') != std::string::npos;
  m_white_castle_queenside = token.find('Q') != std::string::npos;
  m_black_castle_kingside = token.find('k') != std::string::npos;
  m_black_castle_queenside = token.find('q') != std::string::npos;

  // Fourth token: en passant
  iss >> token;
  if (token == "-") {
    // TODO requires better square
  }

  // Fifth token: Halfmove clock
  iss >> m_halfmove_clock;

  // Sixth token: Fullmove number
  iss >> m_fullmove_number;
}

Bitboard Board::white_pieces() const {
  Bitboard bb;
  bb |= m_w_pawns;
  bb |= m_w_rooks;
  bb |= m_w_bishops;
  bb |= m_w_knights;
  bb |= m_w_king;
  bb |= m_w_queen;
  return bb;
}

Bitboard Board::black_pieces() const {
  Bitboard bb;
  bb |= m_b_pawns;
  bb |= m_b_rooks;
  bb |= m_b_bishops;
  bb |= m_b_knights;
  bb |= m_b_king;
  bb |= m_b_queen;
  return bb;
}

Bitboard Board::occupied() const {
  Bitboard bb;
  bb |= m_b_pawns;
  bb |= m_b_rooks;
  bb |= m_b_bishops;
  bb |= m_b_knights;
  bb |= m_b_king;
  bb |= m_b_queen;
  bb |= m_w_pawns;
  bb |= m_w_rooks;
  bb |= m_w_bishops;
  bb |= m_w_knights;
  bb |= m_w_king;
  bb |= m_w_queen;
  return bb;
}

Piece Board::get_piece(Square sq) const {
  if (m_w_pawns.test(sq)) return Piece::P;
  if (m_w_knights.test(sq)) return Piece::N;
  if (m_w_bishops.test(sq)) return Piece::B;
  if (m_w_rooks.test(sq)) return Piece::R;
  if (m_w_queen.test(sq)) return Piece::Q;
  if (m_w_king.test(sq)) return Piece::K;

  if (m_b_pawns.test(sq)) return Piece::p;
  if (m_b_knights.test(sq)) return Piece::n;
  if (m_b_bishops.test(sq)) return Piece::b;
  if (m_b_rooks.test(sq)) return Piece::r;
  if (m_b_queen.test(sq)) return Piece::q;
  if (m_b_king.test(sq)) return Piece::k;

  return Piece::NO_PIECE;
}

void Board::set_piece(Square sq, Piece p) {
  switch (p.type()) {
    case Piece::P:
      m_w_pawns.set(sq);
      break;
    case Piece::N:
      m_w_knights.set(sq);
      break;
    case Piece::B:
      m_w_bishops.set(sq);
      break;
    case Piece::R:
      m_w_rooks.set(sq);
      break;
    case Piece::Q:
      m_w_queen.set(sq);
      break;
    case Piece::K:
      m_w_king.set(sq);
      break;
    case Piece::p:
      m_b_pawns.set(sq);
      break;
    case Piece::n:
      m_b_knights.set(sq);
      break;
    case Piece::b:
      m_b_bishops.set(sq);
      break;
    case Piece::r:
      m_b_rooks.set(sq);
      break;
    case Piece::q:
      m_b_queen.set(sq);
      break;
    case Piece::k:
      m_b_king.set(sq);
      break;
    case Piece::NO_PIECE:
      std::cerr << "NO_PIECE piece cannot not be set\n";
    default:
      throw std::runtime_error("Invalid piece: cannot set piece on the given square");
      break;
  }
}

void Board::remove_piece(Square sq) {
  // clear the square from ALL bitboards (only one will match)
  m_w_pawns.clear(sq);
  m_w_knights.clear(sq);
  m_w_bishops.clear(sq);
  m_w_rooks.clear(sq);
  m_w_queen.clear(sq);
  m_w_king.clear(sq);

  m_b_pawns.clear(sq);
  m_b_knights.clear(sq);
  m_b_bishops.clear(sq);
  m_b_rooks.clear(sq);
  m_b_queen.clear(sq);
  m_b_king.clear(sq);
}

void Board::print(std::ostream& os) const {
  for (int rank = 7; rank >= 0; --rank) {
    os << (rank + 1) << " ";  // rank numbers on the left
    for (int file = 0; file < 8; ++file) {
      Square sq = Square(rank * 8 + file);

      Piece p = get_piece(sq);
      char c = p.to_char();
      os << c << " ";
    }
    os << "\n";
  }
  os << "  a b c d e f g h\n";  // file labels
}
