include(CTest)
find_package(doctest CONFIG REQUIRED)

set(tests_SRC
	test_basic_scenario.cpp
	tests_main.cpp
	# dag/graph.cpp
	# dag/to_index_graph.cpp
	# set/contains.cpp
	# set/indexes.cpp
	# set/index_of.cpp
	# set/types.cpp
)
add_executable(tests ${tests_SRC})
target_include_directories(tests PUBLIC utils)
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