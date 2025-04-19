#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <chess_engine/move.hpp>

namespace chess {

std::string Move::to_uci() const {
    std::string result;
    // convert back to char from int (using the offset of the ASCII table)
    result += static_cast<char>('a' + from.file);
    result += static_cast<char>('1' + from.rank);
    result += static_cast<char>('a' + to.file);
    result += static_cast<char>('1' + to.rank);
    if (is_promotion) {
        switch (promotion_piece) {
            case Piece::QUEEN: result += 'q'; break;
            case Piece::ROOK: result += 'r'; break;
            case Piece::BISHOP: result += 'b'; break;
            case Piece::KNIGHT: result += 'n'; break;
            default: break;
        }
    }
    return result;
}

} // namespace chess