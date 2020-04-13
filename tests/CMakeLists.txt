set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(Warnings)

add_executable(helloworld helloworld/withmess/helloworld.cpp)
target_link_libraries(helloworld PRIVATE mess)
target_set_warnings(helloworld ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	helloworld
	PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS NO
)

add_executable(helloworldwomess helloworld/womess/helloworld.cpp)
target_set_warnings(helloworldwomess ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	helloworldwomess
	PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS NO
)

find_package(doctest CONFIG REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_executable(tests test_main.cpp test_mess.cpp test_helloworld.cpp)
else()
	add_executable(tests test_main.cpp test_mess.cpp)
endif()
target_link_libraries(tests PRIVATE doctest::doctest mess)
target_set_warnings(tests ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	tests
	PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS NO
)

include(CTest)
add_test(NAME mess.tests COMMAND tests -s)