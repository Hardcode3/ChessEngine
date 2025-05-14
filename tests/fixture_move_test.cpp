#include <gtest/gtest.h>

#include <chess_engine/game.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

using namespace chess;

class MoveTest : public ::testing::Test {
 protected:
  Game game;
  std::vector<Move> moves;

  void SetUp() override { moves.clear(); }

  bool contains_move(const Square& from, const Square& to, const Piece& piece,
                     const std::optional<Piece>& promotion = std::nullopt,
                     const std::optional<Piece>& captured = std::nullopt) const {
    for (const auto& move : moves) {
      if (move.get_from() == from && move.get_to() == to && move.get_piece() == piece &&
          move.get_promotion().has_value() == promotion.has_value() &&
          (!move.get_promotion().has_value() || move.get_promoted_to() == promotion.value()) &&
          move.get_captured() == captured) {
        return true;
      }
    }
    return false;
  }
};
