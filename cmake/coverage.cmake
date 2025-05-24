option(ENABLE_COVERAGE "Enable coverage reporting" ON)

# Only enable on Debug builds with supported compilers
if(ENABLE_COVERAGE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling coverage flags")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
        set(COVERAGE_TOOL "llvm")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage -fprofile-arcs -ftest-coverage)
        set(COVERAGE_TOOL "gcov")
    else()
        message(WARNING "Coverage is only supported with GCC or Clang")
        set(ENABLE_COVERAGE OFF)
    endif()
endif()

if(ENABLE_COVERAGE AND COVERAGE_TOOL STREQUAL "llvm")
    set(COVERAGE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/coverage" CACHE PATH "Coverage output directory")
    file(MAKE_DIRECTORY "${COVERAGE_OUTPUT_DIR}")

    set(PROFRAW "${COVERAGE_OUTPUT_DIR}/default.profraw")
    set(PROFDATA "${COVERAGE_OUTPUT_DIR}/default.profdata")

    # Find all test binaries matching "test_*"
    file(GLOB_RECURSE TEST_BINARIES "${CMAKE_BINARY_DIR}/test_*")

    # Generate coverage command for each test binary
    set(COVERAGE_COMMANDS "")
    foreach(TEST_BIN ${TEST_BINARIES})
        list(APPEND COVERAGE_COMMANDS
            COMMAND llvm-cov show ${TEST_BIN}
                -instr-profile=${PROFDATA}
                -format=html
                -use-color
                -Xdemangler c++filt
                -output-dir=${COVERAGE_OUTPUT_DIR}/$(basename ${TEST_BIN})
        )
    endforeach()

    add_custom_target(coverage
        # Run tests and generate profraw
        COMMAND LLVM_PROFILE_FILE=${PROFRAW} ${CMAKE_CTEST_COMMAND} --output-on-failure
        # Merge profraw to profdata
        COMMAND llvm-profdata merge -sparse ${PROFRAW} -o ${PROFDATA}
        # Run llvm-cov on all test binaries
        ${COVERAGE_COMMANDS}
        COMMENT "Generating LLVM coverage reports for all test_* binaries"
        VERBATIM
    )
endif()

if(ENABLE_COVERAGE AND COVERAGE_TOOL STREQUAL "gcov")
    set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/coverage")

    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND lcov --directory . --capture --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' '*/test/*' --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory ${COVERAGE_REPORT_DIR}
        COMMENT "Generating GCOV/LCOV HTML coverage report"
        VERBATIM
    )
endif()
