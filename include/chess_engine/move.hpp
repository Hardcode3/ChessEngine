#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>

namespace chess
{

struct Move
{
  Square from;
  Square to;
  Piece  piece;
  Piece  captured;
  bool   is_promotion;
  Piece  promotion_piece;

  Move ( Square f, Square t, Piece p )
      : from ( f ), to ( t ), piece ( p ), captured ( Piece::EMPTY ), is_promotion ( false ),
        promotion_piece ( Piece::EMPTY )
  {
  }

  Move ( Square f, Square t, Piece p, Piece c )
      : from ( f ), to ( t ), piece ( p ), captured ( c ), is_promotion ( false ), promotion_piece ( Piece::EMPTY )
  {
  }

  Move ( Square f, Square t, Piece p, Piece c, bool prom, Piece prom_piece )
      : from ( f ), to ( t ), piece ( p ), captured ( c ), is_promotion ( prom ), promotion_piece ( prom_piece )
  {
  }

  std::string
  to_uci () const;
};

} // namespace chess