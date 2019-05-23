cmake_minimum_required(VERSION 3.5)

project(mess VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
	set(DOXYFILE_TO_CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYFILE_TO_CONFIGURE} ${DOXYFILE} @ONLY)

	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
endif(DOXYGEN_FOUND)

include_directories(include)

add_executable(helloworld examples/helloworld.cpp)
add_executable(helloworldwomess examples/womess/helloworld.cpp)
add_executable(hellofile examples/hellofile.cpp)

find_package(GTest QUIET)
if(GTEST_FOUND)
    enable_testing()
    add_subdirectory("tests")
endif()

install(DIRECTORY include/mess DESTINATION include)