#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace chess
{

/**
 * In chess, the terms "file" and "rank" are the standard terminology used to
 * describe the coordinates of a square on the chessboard:
 * - A file is a vertical column of squares (labeled a through h from left to
 * right) -A rank is a horizontal row of squares (labeled 1 through 8 from
 * bottom to top)
 *
 * This terminology comes from the traditional chess notation system. For
 * example: The square "e4" means:
 * - File: 'e' (the 5th column from the left)
 * - Rank: '4' (the 4th row from the bottom)
 *
 * While we could use "row" and "column" instead, using "file" and "rank" has
 * several advantages:
 * - It's the standard terminology in chess literature and programming
 * - It matches the algebraic notation used in chess (e.g., e4, Nf3)
 * - It's more precise in the context of chess, as "row" and "column" could be
 * ambiguous about which direction is which
 */
struct Square
{
  uint8_t file; // lines 0-7 (a-h)
  uint8_t rank; // lines 0-7 (1-8)

  Square ( uint8_t f, uint8_t r ) : file ( f ), rank ( r )
  {
  }
  Square ( const std::string &notation ); // e.g., "e4"

  bool
  is_valid () const
  {
    return file >= 0 && file < 8 && rank >= 0 && rank < 8;
  }

  bool
  operator== ( const Square &other ) const
  {
    return file == other.file && rank == other.rank;
  }

  std::string
  to_string () const
  {
    return std::string ( 1, 'a' + file ) + std::to_string ( rank + 1 );
  }
};

} // namespace chess