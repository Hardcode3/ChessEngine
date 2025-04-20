#include <gtest/gtest.h>
#include <chess_engine/square.hpp>

using namespace chess;


TEST(SquareTest, ConstructorFromFileRankValid) {
    const std::string files = "abcdefgh";
    const std::string ranks = "12345678";

    for (char file : files) {
        for (char rank : ranks) {
            std::string notation = std::string(1, file) + std::string(1, rank);
            uint8_t file_index = file - 'a';
            uint8_t rank_index = rank - '1';

            Square square_from_notation(notation);
            Square square_from_indices(file_index, rank_index);

            EXPECT_EQ(square_from_notation, square_from_indices)
                << "Failed for square " << notation;
            EXPECT_EQ(square_from_notation.to_string(), notation)
                << "Failed for square " << notation;
            EXPECT_EQ(square_from_indices.to_string(), notation)
                << "Failed for square " << notation;
            EXPECT_EQ(square_from_notation.file, file_index)
                << "Failed for square " << notation;
            EXPECT_EQ(square_from_notation.rank, rank_index)
                << "Failed for square " << notation;
        }
    }
}

TEST(SquareTest, ConstructorFromFileRankInvalid) {
    struct TestCase {
        uint8_t file;
        uint8_t rank;
        std::string description;
    };

    const std::vector<TestCase> test_cases = {
        {8, 0, "file out of bounds"},
        {5, 8, "rank out of bounds"},
        {17, 8, "both file and rank out of bounds"}
    };

    for (const auto& test_case : test_cases) {
        auto create_invalid_square = [&test_case]() {
            Square s(test_case.file, test_case.rank);
        };
        EXPECT_THROW(create_invalid_square(), std::invalid_argument)
            << "Failed for case: " << test_case.description
            << " (file=" << static_cast<int>(test_case.file)
            << ", rank=" << static_cast<int>(test_case.rank) << ")";
    }
}

TEST(SquareTest, ConstructorFromNotationValid) {
    struct TestCase {
        std::string notation;
        uint8_t expected_file;
        uint8_t expected_rank;
    };

    const std::vector<TestCase> test_cases = {
        {"a1", 0, 0},
        {"h8", 7, 7},
        {"a8", 0, 7},
        {"h1", 7, 0},
        {"e4", 4, 3},
    };

    for (const auto& test_case : test_cases) {
        Square square(test_case.notation);
        EXPECT_EQ(square.file, test_case.expected_file)
            << "Failed for notation: " << test_case.notation;
        EXPECT_EQ(square.rank, test_case.expected_rank)
            << "Failed for notation: " << test_case.notation;
    }
}

TEST(SquareTest, ConstructorFromNotationInvalid) {
    struct TestCase {
        std::string notation;
        std::string description;
    };

    const std::vector<TestCase> test_cases = {
        {"i1", "invalid file"},
        {"a9", "invalid rank"},
        {"a", "too short"},
        {"a12", "too long"},
        {"z3", "invalid file"},
        {"", "empty string"},
        {" ", "invalid notation"},
        {"  ", "invalid notation"},
        {" 5", "invalid notation"},
        {"a ", "invalid notation"},
    };

    for (const auto& test_case : test_cases) {
        EXPECT_THROW(Square(test_case.notation), std::invalid_argument)
            << "Failed for case: " << test_case.description
            << " (notation=" << test_case.notation << ")";
    }
}

TEST(SquareTest, ToStringValid) {
    struct TestCase {
        uint8_t file;
        uint8_t rank;
        std::string expected_notation;
    };

    const std::vector<TestCase> test_cases = {
        {0, 0, "a1"},
        {7, 7, "h8"},
        {0, 7, "a8"},
        {7, 0, "h1"},
        {4, 3, "e4"},
    };

    for (const auto& test_case : test_cases) {
        Square square(test_case.file, test_case.rank);
        EXPECT_EQ(square.to_string(), test_case.expected_notation)
            << "Failed for square at file=" << static_cast<int>(test_case.file)
            << ", rank=" << static_cast<int>(test_case.rank);
    }
}

TEST(SquareTest, EqualityOperator) {
    struct TestCase {
        Square square1;
        Square square2;
        bool expected_equal;
        std::string description;
    };

    const std::vector<TestCase> test_cases = {
        {Square(0, 0), Square(0, 0), true, "same square"},
        {Square(0, 0), Square(4, 3), false, "different squares"},
        {Square(0, 0), Square(7, 7), false, "different squares"}
    };

    for (const auto& test_case : test_cases) {
        EXPECT_EQ(test_case.square1 == test_case.square2, test_case.expected_equal)
            << "Failed for case: " << test_case.description;
    }
}

TEST(SquareTest, InequalityOperator) {
    struct TestCase {
        Square square1;
        Square square2;
        bool expected_unequal;
        std::string description;
    };

    const std::vector<TestCase> test_cases = {
        {Square(0, 0), Square(0, 0), false, "same square"},
        {Square(0, 0), Square(4, 3), true, "different squares"},
        {Square(0, 0), Square(7, 7), true, "different squares"}
    };

    for (const auto& test_case : test_cases) {
        EXPECT_EQ(test_case.square1 != test_case.square2, test_case.expected_unequal)
            << "Failed for case: " << test_case.description;
    }
}

