message(STATUS "> Found tests directory")

file(GLOB TESTS_SOURCES "*.cpp")
file(GLOB TESTS_HEADERS "*.hpp")

foreach(TEST_FILE ${TESTS_SOURCES})
    # Get file name (Without Extension)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${TESTS_HEADERS})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE ChessEngineLib GTest::gtest GTest::gtest_main)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET ${TEST_NAME} PROPERTY FOLDER Tests)
endforeach(TEST_FILE)
