#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <chess_engine/board.hpp>

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
      fullmove_number(1)
{}


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
void Board::load_fen(const std::string& fen) {
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
            file += c - '0'; // convert token to index (using ASCII)
        } else {
            Color color = isupper(c) ? Color::WHITE : Color::BLACK;
            c = tolower(c);
            Piece piece;
            switch (c) {
                case 'p': piece = Piece::PAWN; break;
                case 'n': piece = Piece::KNIGHT; break;
                case 'b': piece = Piece::BISHOP; break;
                case 'r': piece = Piece::ROOK; break;
                case 'q': piece = Piece::QUEEN; break;
                case 'k': piece = Piece::KING; break;
                default: throw std::invalid_argument("Invalid FEN piece");
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
    // Use remove_if to reorder the vector so that all elements that should be removed are moved to the end.
    // Then, erase the elements at the end of the vector from the new iterator til the end of the vector.
    moves.erase(
        std::remove_if(moves.begin(), moves.end(), [this](const Move& move) { return !this->is_move_legal(move); }),
        moves.end()
    );

    return moves;
}


void Board::make_move(Move& move) {
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
    }

    // Update game state
    this->side_to_move = (this->side_to_move == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (this->side_to_move == Color::WHITE) {
        ++this->fullmove_number;
    }
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
        0,  // EMPTY
        100, // PAWN
        320, // KNIGHT
        330, // BISHOP
        500, // ROOK
        900, // QUEEN
        20000 // KING
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


/**
 * Universal Chess Interface (UCI) Move Format
 *
 * UCI moves are represented as strings in the format:
 * - Normal moves: "e2e4" (from square to square)
 * - Promotions: "a7a8q" (with piece letter: q=queen, r=rook, b=bishop, n=knight)
 * - Castling: "e1g1" (white kingside), "e1c1" (white queenside)
 *
 * The function parses these strings into Move objects, handling:
 * - Square validation
 * - Piece identification
 * - Promotion piece specification
 */
Move Board::uci_to_move(const std::string& uci_move) const {
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
            case 'q': move.promotion_piece = Piece::QUEEN; break;
            case 'r': move.promotion_piece = Piece::ROOK; break;
            case 'b': move.promotion_piece = Piece::BISHOP; break;
            case 'n': move.promotion_piece = Piece::KNIGHT; break;
            default: throw std::invalid_argument("Invalid promotion piece");
        }
    }

    return move;
}

std::string Board::move_to_uci(const Move& move) const {
    return move.to_uci();
}

bool Board::is_game_over() const {
    // Check for checkmate or stalemate
    std::vector<Move> moves = this->generate_legal_moves();
    return moves.empty();
}

void Board::generate_pawn_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

void Board::generate_knight_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

void Board::generate_bishop_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

void Board::generate_rook_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

void Board::generate_queen_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

void Board::generate_king_moves(std::vector<Move>& moves, Square square) const {
    // TODO: Implement
}

} // namespace chess