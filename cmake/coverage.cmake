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
