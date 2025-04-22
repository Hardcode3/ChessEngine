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

  std::istringstream iss(fen);
  std::string token;

  // Piece placement (starting from a8)
  // from top to bottom and left to right
  uint8_t file = 0;
  uint8_t rank = 0;
  iss >> token;

  // Parsing the first part of the FEN string e.g. rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
  for (auto c : token) {
    if (c == '/') {
      rank++;
      file = 0;
    } else if (isdigit(c)) {
      file += c - '0';
    } else {
      const Square square(file, rank);
      Piece piece(c);
      std::cout << "Setting piece " << piece.to_char() << " to " << square.to_string() << std::endl;
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
    this->state.en_passant_square = Square(token);
  }

  iss >> this->state.halfmove_clock;
  iss >> this->state.fullmove_number;
}

}  // namespace chess