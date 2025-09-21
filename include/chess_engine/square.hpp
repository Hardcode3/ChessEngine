#pragma once
#include <string>

/**
 * @brief Represents a square on the chess board.
 *
 * Internally wraps an enum with values 0–63 for squares A1–H8.
 * Provides convenient constructors, accessors, and comparison operators.
 */
class Square {
 public:
  /**
   * @brief Enum representing squares of a chess board.
   *
   * Mapping is row-major: A1 = 0 and H8 = 63.
   *
   * @note An unscoped enum is used because:
   * - Square values must be implicitly convertible to int
   * - This allows efficient use in bitboards and shifts
   * - Scoped enums (`enum class`) forbid implicit conversions
   */
  // clang-format off
  enum Value : int {
      A1, B1, C1, D1, E1, F1, G1, H1,
      A2, B2, C2, D2, E2, F2, G2, H2,
      A3, B3, C3, D3, E3, F3, G3, H3,
      A4, B4, C4, D4, E4, F4, G4, H4,
      A5, B5, C5, D5, E5, F5, G5, H5,
      A6, B6, C6, D6, E6, F6, G6, H6,
      A7, B7, C7, D7, E7, F7, G7, H7,
      A8, B8, C8, D8, E8, F8, G8, H8,
    };
  // clang-format on

 private:
  /// @brief The internal square value (0–63).
  Value m_value;

 public:
  /**
   * @brief Construct a Square from a raw integer value (flattened index).
   * @param v Integer in range [0, 63].
   * @throw std::invalid_argument if v is out of range.
   */
  explicit Square(int v);

  /**
   * @brief Construct a Square directly from an enum value.
   * @param v A valid Value from the Square::Value enum.
   */
  explicit Square(Value v);

  /**
   * @brief Construct a Square from file and rank coordinates.
   * @param file File index (0 = 'a', …, 7 = 'h').
   * @param rank Rank index (0 = '1', …, 7 = '8').
   * @throw std::invalid_argument if file or rank are out of range.
   */
  explicit Square(int file, int rank);

  /**
   * @brief Construct a Square from algebraic notation.
   * @param s String like "e4" (case-insensitive).
   * @throw std::invalid_argument if the string is malformed or out of range.
   */
  explicit Square(const std::string& s);

  /**
   * @brief Get the underlying enum value.
   * @return Square::Value corresponding to this square.
   */
  Value value() const;

  /**
   * @brief Get the file index (column).
   * @return Integer 0–7, where 0 = 'a' and 7 = 'h'.
   */
  int file() const;

  /**
   * @brief Get the rank index (row).
   * @return Integer 0–7, where 0 = '1' and 7 = '8'.
   */
  int rank() const;

  /**
   * @brief Convert the square to algebraic notation.
   * @return Lowercase string like "a1", "e4", "h8".
   */
  std::string to_string() const;

  /**
   * @brief Equality operator.
   * @param other Another Square.
   * @return true if both refer to the same square.
   */
  bool operator==(const Square& other) const;

  /**
   * @brief Inequality operator.
   * @param other Another Square.
   * @return true if the squares differ.
   */
  bool operator!=(const Square& other) const;
};
