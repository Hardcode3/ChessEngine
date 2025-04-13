cmake_minimum_required(VERSION 3.10)

project(CppChessEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies (installed by vcpkg)
find_package(fmt CONFIG REQUIRED)

# Add the executable
add_executable(CppChessEngine main.cpp)

# Link libraries
target_link_libraries(CppChessEngine PRIVATE fmt::fmt)
