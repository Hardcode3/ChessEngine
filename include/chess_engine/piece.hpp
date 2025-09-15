
/**
 * Enum representing pieces of a chess board.
 *
 * By convention:
 * - White are uppercase
 * - Black are lowercase
 *
 * NO_PIECE depicts the absence of a piece.
 *
 * Why using an unscoped enum?
 * - square enum must be interpreted as an int to perform bit shifts
 * - implicit conversions must therefore be feasible
 * - scoped enums (enum class ...) do not allow implicit conversions
 */
enum Piece : int {
  P,
  N,
  B,
  R,
  Q,
  K,  // White
  p,
  n,
  b,
  r,
  q,
  k,  // Black
  NO_PIECE
};
