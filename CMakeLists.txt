cmake_minimum_required(VERSION 3.10)

project(
    CppChessEngine
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Basic C++ chess engine project."
    HOMEPAGE_URL "https://github.com/Hardcode3/CppChessEngine"
)
configure_file(${PROJECT_SOURCE_DIR}/cmake/ChessEngine.h.in ${PROJECT_BINARY_DIR}/ChessEngine.h)

add_definitions(-DCHESS_ENGINE_VERSION=${CMAKE_PROJECT_VERSION})
add_definitions(-DCHESS_ENGINE_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_definitions(-DCHESS_ENGINE_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_definitions(-DCHESS_ENGINE_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})
add_definitions(-DCHESS_ENGINE_ROOT_DIR=${PROJECT_SOURCE_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activate folders to sort targets (useful on Visual Studio for instance)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(COMPILE_EXEC "Copile executable targets or discard them" ON)
set(COMPILE_TESTS "Compile tests or discard them" ON)

include(cmake/clang_format.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src)

if (COMPILE_EXEC)
    add_subdirectory(main)
endif ()

if (COMPILE_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(tests)
endif ()
