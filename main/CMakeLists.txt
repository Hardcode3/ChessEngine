message(STATUS "> Found main directory")

add_executable(SandBox sandbox.cpp)
target_link_libraries(SandBox PRIVATE ChessEngineLib spdlog::spdlog)

add_executable(UCIChessEngine uci_loop.cpp)
target_link_libraries(UCIChessEngine PRIVATE ChessEngineLib spdlog::spdlog)

set_property(
    TARGET
        SandBox
        UCIChessEngine
    PROPERTY FOLDER executables
)
