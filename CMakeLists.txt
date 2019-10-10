cmake_minimum_required(VERSION 3.1)

project(mess VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# if (CMAKE_VERSION VERSION_GREATER "3.9")
# 	cmake_policy(SET CMP0069 NEW)
# 	include(CheckIPOSupported)
# 	check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
# 	if (ipo_supported)
# 		message(STATUS "IPO / LTO enabled")
# 		set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
# 		set_property(TARGET helloworldsplit PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
# 		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# 	else()
# 		message(STATUS "IPO / LTO not supported: <${ipo_error}>")
# 	endif()
# endif()

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
	set(DOXYFILE_TO_CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYFILE_TO_CONFIGURE} ${DOXYFILE} @ONLY)

	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
endif(DOXYGEN_FOUND)

include_directories(include)

add_executable(helloworld examples/helloworld.cpp)
# add_executable(helloworldwomess examples/womess/helloworld.cpp)
# add_executable(helloworldunknown examples/unknown/helloworld.cpp)
# add_executable(hellofile examples/hellofile.cpp)
# add_executable(helloworldsplit examples/split/helloworld.cpp examples/split/logger.cpp)

find_package(GTest QUIET)
if(GTEST_FOUND)
    enable_testing()
    # add_subdirectory("tests")
endif()

install(DIRECTORY include/mess DESTINATION include)