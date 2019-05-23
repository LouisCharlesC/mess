message(STATUS "Building Tests")

add_executable(tests tests.cpp test_mess.cpp)
target_link_libraries(tests PRIVATE gmock GTest::GTest)
add_test(NAME Tests COMMAND tests)