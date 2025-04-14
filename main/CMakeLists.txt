message(STATUS "> Found main directory")

add_executable(SandBox sandbox.cpp)
target_link_libraries(SandBox PRIVATE ChessEngineLib fmt::fmt spdlog::spdlog)

set_property(
    TARGET
        SandBox
    PROPERTY FOLDER executables
)
