include(CTest)
find_package(doctest CONFIG REQUIRED)

add_executable(helloworld helloworld/withmess/helloworld.cpp)
target_include_directories(helloworld PUBLIC $<TARGET_PROPERTY:mess,INCLUDE_DIRECTORIES>)
target_set_warnings(helloworld ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	helloworld
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

add_executable(helloworldwomess helloworld/womess/helloworld.cpp)
target_set_warnings(helloworldwomess ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	helloworldwomess
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

set(tests_SRC tests_main.cpp sequence/test_sequence.cpp)#meta/test_contains.cpp meta/test_root_nodes.cpp meta/test_ordered_predecessors.cpp meta/test_find.cpp)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	list(APPEND tests_SRC test_helloworld.cpp)
endif()
add_executable(tests ${tests_SRC})
target_link_libraries(tests PUBLIC doctest::doctest mess PRIVATE -fsanitize=undefined,address -fno-omit-frame-pointer)
target_set_warnings(tests ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	tests
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

add_test(NAME mess.tests COMMAND tests -s)