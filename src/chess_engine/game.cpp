#include <chess_engine/game.hpp>
#include <sstream>
#include <iostream>


namespace chess {

Game::Game() {
  board.clear();
  state.side_to_move = Piece::Color::WHITE;
}

void Game::from_fen(const std::string& fen) {
  // Example FEN for starting position: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

  // Reset the board before loading the new FEN
  board.clear();

  std::istringstream iss(fen);
  std::string token;

  // Piece placement (starting from a8)
  // from top to bottom and left to right
  uint8_t file = 0;
  uint8_t rank = 7;
  iss >> token;

  // Parsing the first part of the FEN string e.g. rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
  for (auto c : token) {
    if (c == '/') {
      rank--;
      file = 0;
    } else if (isdigit(c)) {
      file += c - '0';
    } else {
      const Square square(file, rank);
      Piece piece(c);
      board.set_piece(square, piece);
      ++file;
    }
  }

  iss >> token;
  this->state.side_to_move = (token == "w") ? Piece::Color::WHITE : Piece::Color::BLACK;

  // Castling rights
  iss >> token;
  this->state.white_can_castle_kingside = token.find('K') != std::string::npos;
  this->state.white_can_castle_queenside = token.find('Q') != std::string::npos;
  this->state.black_can_castle_kingside = token.find('k') != std::string::npos;
  this->state.black_can_castle_queenside = token.find('q') != std::string::npos;

  // En passant
  iss >> token;
  if (token != "-") {
    this->state.en_passant_square.emplace(Square(token));
  }

  iss >> this->state.halfmove_clock;
  iss >> this->state.fullmove_number;
}

std::string Game::get_fen() const {
  std::string fen;

  for (int r = 7; r >= 0; r--) {
    for (uint8_t f = 0; f < BOARD_SIZE; f++) {
      const Piece p = board.get_piece(f, static_cast<uint8_t>(r));
      if (p.type == Piece::Type::EMPTY) {
        if (fen.size() > 0 && isdigit(fen.back())) {
          fen.back() = static_cast<int>(fen.back()) + 1;
        } else {
          fen += '1';
        }
      } else {
        fen += p.to_char();
      }
    }
    if (r != 0) {
      fen += '/';
    }
  }

  fen += ' ';
  fen += (this->get_side_to_move() == Piece::Color::WHITE) ? 'w' : 'b';

  // Castling rights
  fen += ' ';
  if (this->state.white_can_castle_kingside) {
    fen += 'K';
  }
  if (this->state.white_can_castle_queenside) {
    fen += 'Q';
  }
  if (this->state.black_can_castle_kingside) {
    fen += 'k';
  }
  if (this->state.black_can_castle_queenside) {
    fen += 'q';
  }
  if (fen.back() == ' ') {
    fen += '-';
  }

  // En passant
  fen += ' ';
  fen += (this->state.en_passant_square.has_value() ? this->state.en_passant_square.value().to_string() : "-");

  fen += ' ';
  fen += std::to_string(this->state.halfmove_clock);
  fen += ' ';
  fen += std::to_string(this->state.fullmove_number);

  return fen;
}

std::vector<Move> Game::generate_legal_moves() const {
  std::vector<Move> legal_moves;
  const Piece::Color side_to_move = state.side_to_move;

  for (uint8_t rank = 0; rank < BOARD_SIZE; ++rank) {
    for (uint8_t file = 0; file < BOARD_SIZE; ++file) {
      Square square(file, rank);
      const Piece piece = board.get_piece(square);

      // Skip empty squares and pieces of the wrong color
      if (piece.type == Piece::Type::EMPTY || piece.color != side_to_move) {
        continue;
      }

      // Generate moves based on piece type
      switch (piece.type) {
        case Piece::Type::PAWN:
          generate_pawn_moves(legal_moves, square);
          break;
        case Piece::Type::KNIGHT:
          generate_knight_moves(legal_moves, square);
          break;
        case Piece::Type::BISHOP:
          // generate_bishop_moves(legal_moves, square);
          break;
        case Piece::Type::ROOK:
          // generate_rook_moves(legal_moves, square);
          break;
        case Piece::Type::QUEEN:
          // generate_queen_moves(legal_moves, square);
          break;
        case Piece::Type::KING:
          // generate_king_moves(legal_moves, square);
          break;
        case Piece::Type::EMPTY:
          break;
        default:
          break;
      }
    }
  }

  return legal_moves;
  // Filter out moves that would leave the king in check
  // std::vector<Move> filtered_moves;
  // for (const auto& move : legal_moves) {
  //   if (is_move_legal(move)) {
  //     filtered_moves.push_back(move);
  //   }
  // }

  // return filtered_moves;
}

void Game::print_board() const noexcept {
  std::cout << board << "\n";
}

}  // namespace chess