
#include <chess_engine/square.hpp>

namespace chess {

struct GameState {
  bool white_can_castle_kingside = false;
  bool white_can_castle_queenside = false;
  bool black_can_castle_kingside = false;
  bool black_can_castle_queenside = false;
  bool white_to_move = true;
  Square en_passant_target = Square(0, 0);
  int halfmove_clock = 0;
};

}  // namespace chess