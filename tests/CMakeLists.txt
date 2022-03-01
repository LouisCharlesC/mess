include(CTest)
find_package(doctest CONFIG REQUIRED)

add_executable(tests tests_main.cpp tests.cpp)
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