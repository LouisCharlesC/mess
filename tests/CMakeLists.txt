message(STATUS "Building Tests")

find_package(Threads REQUIRED)

add_executable(tests test_mess.cpp)
target_link_libraries(tests PRIVATE "${GTEST_BOTH_LIBRARIES}" Threads::Threads)
add_test(NAME Tests COMMAND tests)