#include <algorithm>
#include <chess_engine/board.hpp>
#include <sstream>
#include <stdexcept>

namespace chess {

Board::Board()
    : pieces{{{Piece::EMPTY}}},  // Initializes all elements to Piece::EMPTY
      colors{{{Color::WHITE}}},  // Initializes all elements to Color::WHITE
      side_to_move(Color::WHITE),
      white_castle_kingside(true),
      white_castle_queenside(true),
      black_castle_kingside(true),
      black_castle_queenside(true),
      en_passant_square(0, 0),
      halfmove_clock(0),
      fullmove_number(1) {}

/**
 * https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation
 * FEN (Forsyth-Edwards Notation) Structure:
 *
 * A FEN string consists of 6 parts separated by spaces:
 * 1. Piece Placement: Describes the position of pieces on the board
 *    - Uppercase letters for white pieces (PNBRQK)
 *    - Lowercase letters for black pieces (pnbrqk)
 *    - Numbers represent empty squares
 *    - '/' separates ranks (rows)
 *    - Example: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
 *
 * 2. Active Color: 'w' for white to move, 'b' for black
 *
 * 3. Castling Rights: Combination of KQkq or '-'
 *    - K: White can castle kingside
 *    - Q: White can castle queenside
 *    - k: Black can castle kingside
 *    - q: Black can castle queenside
 *
 * 4. En Passant: Square where en passant capture is possible or '-'
 *    - Example: "e3" if a pawn just moved two squares
 *
 * 5. Halfmove Clock: Number of half-moves since last capture or pawn move
 *    - Used for the 50-move rule
 *
 * 6. Fullmove Number: Current move number, starting at 1
 *    - Increments after black's move
 *
 * Example FEN for starting position:
 * "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
 */
void Board::load_fen(const std::string &fen) {
  std::istringstream iss(fen);
  std::string token;

  // Piece placement (starting from e8)
  // from top to bottom and left to right
  int rank = 7;
  int file = 0;
  iss >> token;

  // Parsing the first part of the FEN string
  // e.g. rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
  // if the character is a slash, move to the next rank and reset the file
  // if the character is a digit, skip the number of squares
  // otherwise, add the piece to the board
  for (auto c : token) {
    if (c == '/') {
      --rank;
      file = 0;
    } else if (isdigit(c)) {
      file += c - '0';  // convert token to index (using ASCII)
    } else {
      Color color = isupper(c) ? Color::WHITE : Color::BLACK;
      c = tolower(c);
      Piece piece;
      switch (c) {
        case 'p':
          piece = Piece::PAWN;
          break;
        case 'n':
          piece = Piece::KNIGHT;
          break;
        case 'b':
          piece = Piece::BISHOP;
          break;
        case 'r':
          piece = Piece::ROOK;
          break;
        case 'q':
          piece = Piece::QUEEN;
          break;
        case 'k':
          piece = Piece::KING;
          break;
        default:
          throw std::invalid_argument("Invalid FEN piece");
      }
      this->pieces[rank][file] = piece;
      this->colors[rank][file] = color;
      ++file;
    }
  }

  // Side to move
  iss >> token;
  this->side_to_move = (token == "w") ? Color::WHITE : Color::BLACK;

  // Castling rights
  iss >> token;
  this->white_castle_kingside = token.find('K') != std::string::npos;
  this->white_castle_queenside = token.find('Q') != std::string::npos;
  this->black_castle_kingside = token.find('k') != std::string::npos;
  this->black_castle_queenside = token.find('q') != std::string::npos;

  // En passant
  iss >> token;
  if (token != "-") {
    this->en_passant_square = Square(token);
  }

  // Halfmove clock
  iss >> this->halfmove_clock;

  // Fullmove number
  iss >> this->fullmove_number;
}

std::string Board::get_fen() const {
  std::string fen;

  // Piece placement
  for (int rank = 7; rank >= 0; --rank) {
    int empty_count = 0;
    for (int file = 0; file < 8; ++file) {
      Piece piece = pieces[rank][file];
      if (piece == Piece::EMPTY) {
        ++empty_count;
      } else {
        if (empty_count > 0) {
          fen += std::to_string(empty_count);
          empty_count = 0;
        }
        char piece_char;
        switch (piece) {
          case Piece::PAWN:
            piece_char = 'p';
            break;
          case Piece::KNIGHT:
            piece_char = 'n';
            break;
          case Piece::BISHOP:
            piece_char = 'b';
            break;
          case Piece::ROOK:
            piece_char = 'r';
            break;
          case Piece::QUEEN:
            piece_char = 'q';
            break;
          case Piece::KING:
            piece_char = 'k';
            break;
          default:
            piece_char = ' ';
        }
        if (colors[rank][file] == Color::WHITE) {
          piece_char = toupper(piece_char);
        }
        fen += piece_char;
      }
    }
    if (empty_count > 0) {
      fen += std::to_string(empty_count);
    }
    if (rank > 0) {
      fen += '/';
    }
  }

  // Side to move
  fen += (side_to_move == Color::WHITE) ? " w " : " b ";

  // Castling rights
  std::string castling;
  if (white_castle_kingside) castling += 'K';
  if (white_castle_queenside) castling += 'Q';
  if (black_castle_kingside) castling += 'k';
  if (black_castle_queenside) castling += 'q';
  if (castling.empty()) castling = "-";
  fen += castling + " ";

  // En passant
  if (en_passant_square.is_valid()) {
    fen += en_passant_square.to_string() + " ";
  } else {
    fen += "- ";
  }

  // Halfmove clock and fullmove number
  fen += std::to_string(halfmove_clock) + " " + std::to_string(fullmove_number);

  return fen;
}

/**
 * Universal Chess Interface (UCI) Move Format
 *
 * UCI moves are represented as strings in the format:
 * - Normal moves: "e2e4" (from square to square)
 * - Promotions: "a7a8q" (with piece letter: q=queen, r=rook, b=bishop,
 * n=knight)
 * - Castling: "e1g1" (white kingside), "e1c1" (white queenside)
 *
 * The function parses these strings into Move objects, handling:
 * - Square validation
 * - Piece identification
 * - Promotion piece specification
 */
Move Board::uci_to_move(const std::string &uci_move) const {
  if (uci_move.length() < 4 || uci_move.length() > 5) {
    throw std::invalid_argument("Invalid UCI move format");
  }

  Square from(uci_move.substr(0, 2));
  Square to(uci_move.substr(2, 2));
  Piece piece = this->get_piece(from);
  Move move(from, to, piece);

  // Handle promotion
  if (uci_move.length() == 5) {
    move.is_promotion = true;
    switch (uci_move[4]) {
      case 'q':
        move.promotion_piece = Piece::QUEEN;
        break;
      case 'r':
        move.promotion_piece = Piece::ROOK;
        break;
      case 'b':
        move.promotion_piece = Piece::BISHOP;
        break;
      case 'n':
        move.promotion_piece = Piece::KNIGHT;
        break;
      default:
        throw std::invalid_argument("Invalid promotion piece");
    }
  }

  return move;
}

std::string Board::move_to_uci(const Move &move) const { return move.to_uci(); }

std::vector<Move> Board::generate_legal_moves() const {
  std::vector<Move> moves;
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Square square(file, rank);
      if (this->get_piece(square) != Piece::EMPTY && this->get_color(square) == side_to_move) {
        switch (this->get_piece(square)) {
          case Piece::PAWN:
            this->generate_pawn_moves(moves, square);
            break;
          case Piece::KNIGHT:
            this->generate_knight_moves(moves, square);
            break;
          case Piece::BISHOP:
            this->generate_bishop_moves(moves, square);
            break;
          case Piece::ROOK:
            this->generate_rook_moves(moves, square);
            break;
          case Piece::QUEEN:
            this->generate_queen_moves(moves, square);
            break;
          case Piece::KING:
            this->generate_king_moves(moves, square);
            break;
          default:
            break;
        }
      }
    }
  }

  // Filter out illegal moves
  moves.erase(
      std::remove_if(moves.begin(), moves.end(), [this](const Move &move) { return !this->is_move_legal(move); }),
      moves.end());

  return moves;
}

void Board::make_move(Move &move) {
  // Store state before the move
  move.white_castle_kingside = white_castle_kingside;
  move.white_castle_queenside = white_castle_queenside;
  move.black_castle_kingside = black_castle_kingside;
  move.black_castle_queenside = black_castle_queenside;
  move.previous_en_passant = en_passant_square;
  move.previous_halfmove_clock = halfmove_clock;

  // Store captured piece and move the piece
  move.captured = this->get_piece(move.to);
  this->pieces[move.to.rank][move.to.file] = move.piece;
  this->colors[move.to.rank][move.to.file] = this->side_to_move;
  this->pieces[move.from.rank][move.from.file] = Piece::EMPTY;

  // Handle special moves
  if (move.piece == Piece::PAWN) {
    // Handle en passant
    if (move.to == this->en_passant_square) {
      this->pieces[move.to.rank - (this->side_to_move == Color::WHITE ? 1 : -1)][move.to.file] = Piece::EMPTY;
    }
    // Handle promotion
    if (move.is_promotion) {
      this->pieces[move.to.rank][move.to.file] = move.promotion_piece;
    }
  } else if (move.is_castling) {
    // Move the rook
    this->pieces[move.castling_rook_to.rank][move.castling_rook_to.file] = Piece::ROOK;
    this->colors[move.castling_rook_to.rank][move.castling_rook_to.file] = this->side_to_move;
    this->pieces[move.castling_rook_from.rank][move.castling_rook_from.file] = Piece::EMPTY;

    // Update castling rights
    if (this->side_to_move == Color::WHITE) {
      this->white_castle_kingside = false;
      this->white_castle_queenside = false;
    } else {
      this->black_castle_kingside = false;
      this->black_castle_queenside = false;
    }
  } else if (move.piece == Piece::KING) {
    // Update castling rights when king moves
    if (this->side_to_move == Color::WHITE) {
      this->white_castle_kingside = false;
      this->white_castle_queenside = false;
    } else {
      this->black_castle_kingside = false;
      this->black_castle_queenside = false;
    }
  } else if (move.piece == Piece::ROOK) {
    // Update castling rights when rook moves
    if (this->side_to_move == Color::WHITE) {
      if (move.from.file == 0) {
        this->white_castle_queenside = false;
      } else if (move.from.file == 7) {
        this->white_castle_kingside = false;
      }
    } else {
      if (move.from.file == 0) {
        this->black_castle_queenside = false;
      } else if (move.from.file == 7) {
        this->black_castle_kingside = false;
      }
    }
  }

  // Update game state
  this->side_to_move = (this->side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
  if (this->side_to_move == Color::WHITE) {
    ++this->fullmove_number;
  }
}

void Board::unmake_move(const Move &move) {
  // Restore the piece to its original position
  pieces[move.from.rank][move.from.file] = move.piece;
  colors[move.from.rank][move.from.file] = side_to_move;
  pieces[move.to.rank][move.to.file] = move.captured;

  // Restore the captured piece's color if there was a capture
  if (move.captured != Piece::EMPTY) {
    colors[move.to.rank][move.to.file] = (side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
  }

  // Handle special moves
  if (move.piece == Piece::PAWN) {
    // Handle en passant
    if (move.to == en_passant_square) {
      const int capture_rank = (side_to_move == Color::WHITE) ? move.to.rank - 1 : move.to.rank + 1;
      pieces[capture_rank][move.to.file] = Piece::PAWN;
      colors[capture_rank][move.to.file] = (side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    // Handle promotion
    if (move.is_promotion) {
      pieces[move.from.rank][move.from.file] = Piece::PAWN;
    }
  } else if (move.is_castling) {
    // Move the rook back
    pieces[move.castling_rook_from.rank][move.castling_rook_from.file] = Piece::ROOK;
    colors[move.castling_rook_from.rank][move.castling_rook_from.file] = side_to_move;
    pieces[move.castling_rook_to.rank][move.castling_rook_to.file] = Piece::EMPTY;

    // Restore castling rights
    if (side_to_move == Color::WHITE) {
      white_castle_kingside = move.white_castle_kingside;
      white_castle_queenside = move.white_castle_queenside;
    } else {
      black_castle_kingside = move.black_castle_kingside;
      black_castle_queenside = move.black_castle_queenside;
    }
  }

  // Restore en passant square
  en_passant_square = move.previous_en_passant;

  // Restore halfmove clock
  halfmove_clock = move.previous_halfmove_clock;

  // Decrement fullmove number if it's black's turn
  if (side_to_move == Color::BLACK) {
    --fullmove_number;
  }

  // Switch side to move
  side_to_move = (side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

/**
 * Position Evaluation
 *
 * Evaluates the current board position from White's perspective:
 * - Positive score: White is winning
 * - Negative score: Black is winning
 * - Zero: Equal position
 *
 * Current implementation uses material values:
 * - Pawn: 100
 * - Knight/Bishop: ~300
 * - Rook: 500
 * - Queen: 900
 * - King: 20000 (effectively infinite)
 *
 * Future improvements could include:
 * - Piece position tables
 * - Pawn structure
 * - King safety
 * - Mobility
 * - Control of center
 */
int Board::evaluate() const {
  const int piece_values[] = {
      0,     // EMPTY
      100,   // PAWN
      320,   // KNIGHT
      330,   // BISHOP
      500,   // ROOK
      900,   // QUEEN
      20000  // KING
  };

  int score = 0;
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Piece piece = pieces[rank][file];
      if (piece != Piece::EMPTY) {
        int value = piece_values[static_cast<int>(piece)];
        score += (colors[rank][file] == Color::WHITE) ? value : -value;
      }
    }
  }
  return score;
}

bool Board::is_game_over() const {
  // Check if there are any legal moves
  std::vector<Move> legal_moves = generate_legal_moves();
  if (!legal_moves.empty()) {
    return false;
  }

  // If no legal moves, check if it's checkmate or stalemate
  Color current_color = side_to_move;
  Square king_square = get_king_square(current_color);
  if (!is_square_safe(king_square, current_color)) {
    return true;  // Checkmate
  }

  // Check for insufficient material
  bool has_pawns = false;
  bool has_knights = false;
  bool has_bishops = false;
  bool has_rooks = false;
  bool has_queens = false;

  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Square square(file, rank);
      Piece piece = get_piece(square);
      if (piece != Piece::EMPTY && piece != Piece::KING) {
        switch (piece) {
          case Piece::PAWN:
            has_pawns = true;
            break;
          case Piece::KNIGHT:
            has_knights = true;
            break;
          case Piece::BISHOP:
            has_bishops = true;
            break;
          case Piece::ROOK:
            has_rooks = true;
            break;
          case Piece::QUEEN:
            has_queens = true;
            break;
          default:
            break;
        }
      }
    }
  }

  // If there are any pawns, rooks, or queens, the game is not drawn by
  // insufficient material
  if (has_pawns || has_rooks || has_queens) {
    return true;  // Stalemate
  }

  // If there are only kings, it's a draw
  if (!has_knights && !has_bishops) {
    return true;
  }

  // If there is only one minor piece (knight or bishop), it's a draw
  if ((has_knights && !has_bishops) || (!has_knights && has_bishops)) {
    return true;
  }

  return true;  // Stalemate
}

void Board::generate_pawn_moves(std::vector<Move> &moves, Square square) const {
  const Color pawn_color = this->get_color(square);
  const int direction = (pawn_color == Color::WHITE) ? 1 : -1;      // White moves up, black moves down
  const int starting_rank = (pawn_color == Color::WHITE) ? 1 : 6;   // White starts at rank 1, black at rank 6
  const int promotion_rank = (pawn_color == Color::WHITE) ? 7 : 0;  // White promotes at rank 7, black at rank 0

  // Forward moves
  const Square one_forward(square.file, square.rank + direction);
  if (one_forward.rank >= 0 && one_forward.rank < 8 && this->get_piece(one_forward) == Piece::EMPTY) {
    // Check for promotion
    if (one_forward.rank == promotion_rank) {
      moves.emplace_back(square, one_forward, Piece::PAWN, Piece::EMPTY, true, Piece::QUEEN);
      moves.emplace_back(square, one_forward, Piece::PAWN, Piece::EMPTY, true, Piece::ROOK);
      moves.emplace_back(square, one_forward, Piece::PAWN, Piece::EMPTY, true, Piece::BISHOP);
      moves.emplace_back(square, one_forward, Piece::PAWN, Piece::EMPTY, true, Piece::KNIGHT);
    } else {
      moves.emplace_back(square, one_forward, Piece::PAWN);
    }

    // Two squares forward from starting position
    if (square.rank == starting_rank) {
      const Square two_forward(square.file, square.rank + 2 * direction);
      if (this->get_piece(two_forward) == Piece::EMPTY) {
        moves.emplace_back(square, two_forward, Piece::PAWN);
      }
    }
  }

  // Diagonal captures
  for (int file_offset : {-1, 1}) {
    const int target_file = square.file + file_offset;
    if (target_file >= 0 && target_file < 8) {
      const Square capture_square(target_file, square.rank + direction);
      if (capture_square.rank >= 0 && capture_square.rank < 8) {
        const Piece target_piece = this->get_piece(capture_square);

        // Regular capture
        if (target_piece != Piece::EMPTY && this->get_color(capture_square) != pawn_color) {
          if (capture_square.rank == promotion_rank) {
            moves.emplace_back(square, capture_square, Piece::PAWN, target_piece, true, Piece::QUEEN);
            moves.emplace_back(square, capture_square, Piece::PAWN, target_piece, true, Piece::ROOK);
            moves.emplace_back(square, capture_square, Piece::PAWN, target_piece, true, Piece::BISHOP);
            moves.emplace_back(square, capture_square, Piece::PAWN, target_piece, true, Piece::KNIGHT);
          } else {
            moves.emplace_back(square, capture_square, Piece::PAWN, target_piece);
          }
        }

        // En passant capture
        if (capture_square == this->en_passant_square) {
          moves.emplace_back(square, capture_square, Piece::PAWN, Piece::PAWN);
        }
      }
    }
  }
}

void Board::generate_knight_moves(std::vector<Move> &moves, Square square) const {
  const int directions[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  const Color knight_color = this->get_color(square);

  for (const auto &dir : directions) {
    const Square target_square(square.file + dir[0], square.rank + dir[1]);
    const Piece target_piece = this->get_piece(target_square);

    if (target_piece == Piece::EMPTY) {
      moves.emplace_back(square, target_square, Piece::KNIGHT);
    } else {
      if (knight_color != this->get_color(target_square)) {
        moves.emplace_back(square, target_square, Piece::KNIGHT, target_piece);
      }
    }
  }
}

void Board::generate_bishop_moves(std::vector<Move> &moves, Square square) const {
  // Define the four diagonal directions a bishop can move
  const int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  const Color bishop_color = this->get_color(square);

  // Check each direction
  for (const auto &dir : directions) {
    Square new_square(square.file + dir[1], square.rank + dir[0]);

    // Keep moving in the current direction until we hit the edge of the board
    while (new_square.is_valid()) {
      const Piece target_piece = this->get_piece(new_square);

      // If the square is empty, add the move
      if (target_piece == Piece::EMPTY) {
        moves.emplace_back(square, new_square, Piece::BISHOP);
      }
      // Enemy piece encountered, add the capture move
      // And stop search in this direction
      else if (this->get_color(new_square) != bishop_color) {
        moves.emplace_back(square, new_square, Piece::BISHOP);
        break;
      }
      // Own piece encountered, stop the search in this direction
      else {
        break;
      }

      // Update square coordinates for next iteration
      new_square.rank += dir[0];
      new_square.file += dir[1];
    }
  }
}

void Board::generate_rook_moves(std::vector<Move> &moves, Square square) const {
  const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  const Color rook_color = this->get_color(square);

  for (const auto &dir : directions) {
    Square new_square(square.rank + dir[0], square.file + dir[1]);

    while (new_square.is_valid()) {
      const Piece target_piece = this->get_piece(new_square);

      // No enemy piece encountered, add the move
      if (target_piece == Piece::EMPTY) {
        moves.emplace_back(square, new_square, Piece::ROOK);
      }
      // Enemy piece encountered, add the capture move
      // And stop search in this direction
      else if (this->get_color(new_square) != rook_color) {
        moves.emplace_back(square, new_square, Piece::ROOK, target_piece);
        break;
      }
      // Own piece encountered, stop the search in this direction
      else {
        break;
      }

      // Update square coordinates for next iteration
      new_square.rank += dir[0];
      new_square.file += dir[1];
    }
  }
}

void Board::generate_queen_moves(std::vector<Move> &moves, Square square) const {
  const int directions[8][2] = {
      {1, 1}, {1, -1}, {-1, 1}, {-1, -1},  // x diagonal moves
      {1, 0}, {-1, 0}, {0, 1},  {0, -1}    // + vertical and horizontal moves
  };

  const Color queen_color = this->get_color(square);

  for (const auto &dir : directions) {
    Square new_square(square.file + dir[0], square.rank + dir[1]);

    while (new_square.is_valid()) {
      const Piece target_piece = this->get_piece(new_square);

      // No enemy piece encountered, add the move
      if (target_piece == Piece::EMPTY) {
        moves.emplace_back(square, new_square, Piece::QUEEN);
      }
      // Enemy piece encountered, add the capture move
      // And stop search in this direction
      else if (this->get_color(new_square) != queen_color) {
        moves.emplace_back(square, new_square, Piece::QUEEN, target_piece);
        break;
      }
      // Own piece encountered, stop the search in this direction
      else {
        break;
      }

      // Update square coordinates for next iteration
      new_square.file += dir[0];
      new_square.rank += dir[1];
    }
  }
}

void Board::generate_king_moves(std::vector<Move> &moves, Square square) const {
  // Regular king moves in all 8 directions
  const int directions[8][2] = {
      {1, 0},  {1, 1},   {0, 1},  {-1, 1},  // Right, Up-Right, Up, Up-Left
      {-1, 0}, {-1, -1}, {0, -1}, {1, -1}   // Left, Down-Left, Down, Down-Right
  };

  const Color king_color = this->get_color(square);
  const int king_rank = (king_color == Color::WHITE) ? 0 : 7;

  // Generate regular moves
  for (const auto &dir : directions) {
    const Square target_square(square.file + dir[0], square.rank + dir[1]);

    if (target_square.is_valid()) {
      const Piece target_piece = this->get_piece(target_square);

      // Check if the target square is safe (not under attack)
      if (is_square_safe(target_square, king_color)) {
        if (target_piece == Piece::EMPTY) {
          moves.emplace_back(square, target_square, Piece::KING);
        } else if (this->get_color(target_square) != king_color) {
          moves.emplace_back(square, target_square, Piece::KING, target_piece);
        }
      }
    }
  }

  // Generate castling moves if the king hasn't moved and is not in check
  if (square.rank == king_rank && square.file == 4 && !is_square_safe(square, king_color)) {
    // Kingside castling
    if ((king_color == Color::WHITE && this->white_castle_kingside) ||
        (king_color == Color::BLACK && this->black_castle_kingside)) {
      // Check if the squares between king and rook are empty and safe
      bool can_castle = true;
      for (int file = 5; file < 7; ++file) {
        const Square check_square(file, king_rank);
        if (this->get_piece(check_square) != Piece::EMPTY || !this->is_square_safe(check_square, king_color)) {
          can_castle = false;
          break;
        }
      }
      if (can_castle) {
        Square rook_square(7, king_rank);
        if (this->get_piece(rook_square) == Piece::ROOK && this->get_color(rook_square) == king_color) {
          // Create a special castling move
          Move castling_move(square, Square(6, king_rank), Piece::KING);
          castling_move.is_castling = true;
          castling_move.castling_rook_from = rook_square;
          castling_move.castling_rook_to = Square(5, king_rank);
          moves.push_back(castling_move);
        }
      }
    }

    // Queenside castling
    if ((king_color == Color::WHITE && this->white_castle_queenside) ||
        (king_color == Color::BLACK && this->black_castle_queenside)) {
      // Check if the squares between king and rook are empty and safe
      bool can_castle = true;
      for (int file = 1; file < 4; ++file) {
        Square check_square(file, king_rank);
        if (this->get_piece(check_square) != Piece::EMPTY || !this->is_square_safe(check_square, king_color)) {
          can_castle = false;
          break;
        }
      }
      if (can_castle) {
        Square rook_square(0, king_rank);
        if (this->get_piece(rook_square) == Piece::ROOK && this->get_color(rook_square) == king_color) {
          // Create a special castling move
          Move castling_move(square, Square(2, king_rank), Piece::KING);
          castling_move.is_castling = true;
          castling_move.castling_rook_from = rook_square;
          castling_move.castling_rook_to = Square(3, king_rank);
          moves.push_back(castling_move);
        }
      }
    }
  }
}

bool Board::is_square_safe(Square square, Color color) const {
  // Check for pawn attacks
  int pawn_dir = (color == Color::WHITE) ? 1 : -1;
  int pawn_rank = square.rank + pawn_dir;
  if (pawn_rank >= 0 && pawn_rank < 8) {
    // Check left diagonal
    int left_file = square.file - 1;
    if (left_file >= 0) {
      Square left_diag(left_file, pawn_rank);
      if (get_piece(left_diag) == Piece::PAWN && get_color(left_diag) != color) {
        return false;
      }
    }
    // Check right diagonal
    int right_file = square.file + 1;
    if (right_file < 8) {
      Square right_diag(right_file, pawn_rank);
      if (get_piece(right_diag) == Piece::PAWN && get_color(right_diag) != color) {
        return false;
      }
    }
  }

  // Check for knight attacks
  const std::vector<std::pair<int, int>> knight_moves = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                                         {1, -2},  {1, 2},  {2, -1},  {2, 1}};
  for (const auto &[file_offset, rank_offset] : knight_moves) {
    int new_file = square.file + file_offset;
    int new_rank = square.rank + rank_offset;
    if (new_file >= 0 && new_file < 8 && new_rank >= 0 && new_rank < 8) {
      Square new_square(new_file, new_rank);
      if (get_piece(new_square) == Piece::KNIGHT && get_color(new_square) != color) {
        return false;
      }
    }
  }

  // Check for king attacks
  const std::vector<std::pair<int, int>> king_moves = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  for (const auto &[file_offset, rank_offset] : king_moves) {
    int new_file = square.file + file_offset;
    int new_rank = square.rank + rank_offset;
    if (new_file >= 0 && new_file < 8 && new_rank >= 0 && new_rank < 8) {
      Square new_square(new_file, new_rank);
      if (get_piece(new_square) == Piece::KING && get_color(new_square) != color) {
        return false;
      }
    }
  }

  // Check for sliding piece attacks (rook, bishop, queen)
  const std::vector<std::pair<int, int>> sliding_directions = {
      {-1, 0},  {1, 0},  {0, -1}, {0, 1},   // Rook directions
      {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};  // Bishop directions

  for (const auto &[file_dir, rank_dir] : sliding_directions) {
    int new_file = square.file + file_dir;
    int new_rank = square.rank + rank_dir;
    while (new_file >= 0 && new_file < 8 && new_rank >= 0 && new_rank < 8) {
      Square new_square(new_file, new_rank);
      Piece piece = get_piece(new_square);
      if (piece != Piece::EMPTY) {
        if (get_color(new_square) != color) {
          if ((file_dir == 0 || rank_dir == 0)  // Rook direction
              && (piece == Piece::ROOK || piece == Piece::QUEEN)) {
            return false;
          }
          if ((file_dir != 0 && rank_dir != 0)  // Bishop direction
              && (piece == Piece::BISHOP || piece == Piece::QUEEN)) {
            return false;
          }
        }
        break;
      }
      new_file += file_dir;
      new_rank += rank_dir;
    }
  }

  return true;
}

bool Board::is_square_attacked(Square square, Color attacker) const {
  // Check for pawn attacks
  const int pawn_direction = (attacker == Color::WHITE) ? 1 : -1;
  const int pawn_attacks[2][2] = {{-1, pawn_direction}, {1, pawn_direction}};

  for (const auto &attack : pawn_attacks) {
    Square target_square(square.file + attack[0], square.rank + attack[1]);
    if (target_square.is_valid()) {
      Piece piece = get_piece(target_square);
      if (piece == Piece::PAWN && get_color(target_square) == attacker) {
        return true;
      }
    }
  }

  // Check for knight attacks
  const int knight_moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  for (const auto &move : knight_moves) {
    Square target_square(square.file + move[0], square.rank + move[1]);
    if (target_square.is_valid()) {
      Piece piece = get_piece(target_square);
      if (piece == Piece::KNIGHT && get_color(target_square) == attacker) {
        return true;
      }
    }
  }

  // Check for king attacks
  const int king_moves[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  for (const auto &move : king_moves) {
    Square target_square(square.file + move[0], square.rank + move[1]);
    if (target_square.is_valid()) {
      Piece piece = get_piece(target_square);
      if (piece == Piece::KING && get_color(target_square) == attacker) {
        return true;
      }
    }
  }

  // Check for sliding pieces (rook, bishop, queen)
  const int sliding_directions[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  for (const auto &dir : sliding_directions) {
    Square target_square(square.file + dir[0], square.rank + dir[1]);
    while (target_square.is_valid()) {
      Piece piece = get_piece(target_square);
      if (piece != Piece::EMPTY) {
        if (get_color(target_square) != attacker) {
          // Check if the piece can attack in this direction
          if ((dir[0] == 0 || dir[1] == 0) &&  // Rook or Queen attack
              (piece == Piece::ROOK || piece == Piece::QUEEN)) {
            return true;
          }
          if ((dir[0] != 0 && dir[1] != 0) &&  // Bishop or Queen attack
              (piece == Piece::BISHOP || piece == Piece::QUEEN)) {
            return true;
          }
        }
        break;
      }
      target_square.file += dir[0];
      target_square.rank += dir[1];
    }
  }

  return false;
}

bool Board::is_in_check(Color color) const {
  // Find the king
  Square king_square(0, 0);  // Initialize with default coordinates
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Square square(file, rank);
      if (get_piece(square) == Piece::KING && get_color(square) == color) {
        king_square = square;
        break;
      }
    }
  }

  // Check if the king's square is attacked by the opponent
  return is_square_attacked(king_square, (color == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

bool Board::is_move_legal(const Move &move) const {
  // Basic validation
  if (!move.from.is_valid() || !move.to.is_valid()) {
    return false;
  }

  // Check if the piece exists and belongs to the side to move
  if (get_piece(move.from) != move.piece || get_color(move.from) != side_to_move) {
    return false;
  }

  // Check if the destination square is not occupied by a friendly piece
  if (get_piece(move.to) != Piece::EMPTY && get_color(move.to) == side_to_move) {
    return false;
  }

  // Make the move temporarily
  Board temp_board = *this;
  temp_board.make_move(const_cast<Move &>(move));

  // Check if the move leaves the king in check
  if (temp_board.is_in_check(side_to_move)) {
    return false;
  }

  return true;
}

Square Board::get_king_square(Color color) const {
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Square square(file, rank);
      if (get_piece(square) == Piece::KING && get_color(square) == color) {
        return square;
      }
    }
  }
  throw std::runtime_error("King not found");
}
}  // namespace chess