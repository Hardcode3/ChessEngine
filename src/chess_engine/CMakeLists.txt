message(STATUS "> Found src/chess_engine directory")

set(target_name ChessEngineLib)
set(target_folder_name chess_engine)

file(GLOB_RECURSE ROOT_SOURCES "${PROJECT_SOURCE_DIR}/src/${target_folder_name}/*.cpp" )
file(GLOB_RECURSE ROOT_HEADERS "${PROJECT_SOURCE_DIR}/include/${target_folder_name}/*.hpp" )
source_group("ChessEngine/Sources" FILES ${ROOT_SOURCES})
source_group("ChessEngine/Headers" FILES ${ROOT_HEADERS})

add_library(${target_name} STATIC "")
target_sources(${target_name}
    PRIVATE
        ${ROOT_SOURCES}
        ${ROOT_HEADERS}
)
target_link_libraries(${target_name} PRIVATE spdlog::spdlog fmt::fmt)
target_include_directories(${target_name}
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)
set_property(TARGET ${target_name} PROPERTY FOLDER ChessEngineLib)
