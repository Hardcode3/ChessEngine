# ------------------------------------------------------------------------------
# ClangFormat.cmake
#
# Defines a custom CMake target `clang-format` to format source files using
# clang-format if it is available on the system. Automatically finds and formats
# all .cpp and .hpp files under src/ and include/ unless a custom list of files
# is provided via CLANG_FORMAT_SOURCES:
#
# set(CLANG_FORMAT_SOURCES
#     ${CMAKE_SOURCE_DIR}/src/main.cpp
#     ${CMAKE_SOURCE_DIR}/include/utils.hpp
# )
# include(cmake/ClangFormat.cmake)
# ------------------------------------------------------------------------------

# Optional: allow user to override the list of files:
if(NOT CLANG_FORMAT_SOURCES)
    file(GLOB_RECURSE CLANG_FORMAT_SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/main/*.cpp"
    )
endif()

# Find clang-format binary
find_program(CLANG_FORMAT_BIN NAMES clang-format)

if(CLANG_FORMAT_BIN)
    message(STATUS "clang-format found: ${CLANG_FORMAT_BIN}")

    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_BIN}
            -i
            -style=file
            ${CLANG_FORMAT_SOURCES}
        COMMENT "Running clang-format"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found. clang-format target will not be available.")
endif()