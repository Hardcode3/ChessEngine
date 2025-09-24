#pragma once
#include <cstdint>

/**
 * @namespace Bitmasks
 * @brief Defines precomputed bitmasks that can be applied during move generation.
 *
 * @example How are these bitmasks defined?
 *
 * Take file C for instance.
 * Each group of 8 bits defines a row.
 * The third bit corresponds to file C.
 *
 * In binary representation:
 *      0000 0100 0000 0100 ... 0000 0100 0000 0100
 *
 * In hexadecimal representation (0x):
 *      0    4    0    4    ... 0    4    0    4
 *
 * Aka. FILE_C = 0x0404040404040404
 *
 * The suffix ULL (Unsigned Long Long is added to ensure the compiler interprets the
 * hexadecimal value as an unsigned integer of length 64 bits).
 */
namespace Bitmasks {

/**
 * uint64_t mask with ones on file A.
 */
constexpr uint64_t FILE_A = 0x0101010101010101ULL;

/**
 * uint64_t mask with ones on file B.
 */
constexpr uint64_t FILE_B = 0x0202020202020202ULL;

/**
 * uint64_t mask with ones on file C.
 */
constexpr uint64_t FILE_C = 0x0404040404040404ULL;

/**
 * uint64_t mask with ones on file D.
 */
constexpr uint64_t FILE_D = 0x0808080808080808ULL;

/**
 * uint64_t mask with ones on file E.
 */
constexpr uint64_t FILE_E = 0x1010101010101010ULL;

/**
 * uint64_t mask with ones on file F.
 */
constexpr uint64_t FILE_F = 0x2020202020202020ULL;

/**
 * uint64_t mask with ones on file G.
 */
constexpr uint64_t FILE_G = 0x4040404040404040ULL;

/**
 * uint64_t mask with ones on file H.
 */
constexpr uint64_t FILE_H = 0x8080808080808080ULL;

/**
 * uint64_t mask with zeros on file A.
 */
constexpr uint64_t NOT_FILE_A = ~FILE_A;

/**
 * uint64_t mask with zeros on file B.
 */
constexpr uint64_t NOT_FILE_B = ~FILE_B;

/**
 * uint64_t mask with zeros on file G.
 */
constexpr uint64_t NOT_FILE_G = ~FILE_G;

/**
 * uint64_t mask with zeros on file H.
 */
constexpr uint64_t NOT_FILE_H = ~FILE_H;

/**
 * uint64_t mask with zeros on files A and B.
 */
constexpr uint64_t NOT_FILE_AB = NOT_FILE_A & NOT_FILE_B;

/**
 * uint64_t mask with zeros on files G and H.
 */
constexpr uint64_t NOT_FILE_GH = NOT_FILE_G & NOT_FILE_H;

}  // namespace Bitmasks
