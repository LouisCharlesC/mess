include(CTest)
find_package(doctest CONFIG REQUIRED)

# add_library(tests_utils utils/operationcounter.cpp)

set(tests_SRC tests_main.cpp tests.cpp)
add_executable(tests ${tests_SRC})
target_link_libraries(tests doctest::doctest mess)
target_set_warnings(tests ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	tests
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

add_test(NAME mess.tests COMMAND tests -s)