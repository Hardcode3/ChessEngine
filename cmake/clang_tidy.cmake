# ------------------------------------------------------------------------------
# Defines a CMake custom target 'clang-tidy' to run clang-tidy on sources.
# It only works if clang-tidy is found. You can override the list of files using
# the CLANG_TIDY_SOURCES variable before including this script.
# ------------------------------------------------------------------------------

find_program(CLANG_TIDY_BIN NAMES clang-tidy)

# Collect source files if not set explicitly
if(NOT CLANG_TIDY_SOURCES)
    file(GLOB_RECURSE CLANG_TIDY_SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/main/*.cpp"
    )
endif()

if(CLANG_TIDY_BIN)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_BIN}")

    # Generate commands for each file
    set(CLANG_TIDY_COMMANDS "")
    foreach(SRC ${CLANG_TIDY_SOURCES})
        list(APPEND CLANG_TIDY_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E echo "Running clang-tidy on ${SRC}"
            COMMAND ${CLANG_TIDY_BIN} ${SRC}
                -p=${CMAKE_BINARY_DIR}     # Use CMake's compile_commands.json
        )
    endforeach()

    add_custom_target(clang-tidy
        ${CLANG_TIDY_COMMANDS}
        COMMENT "Running clang-tidy on all source files"
        VERBATIM
    )
else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy target.")
endif()
