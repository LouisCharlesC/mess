cmake_minimum_required(VERSION 2.8.8)

if (CMAKE_VERSION VERSION_LESS "3.1")
  add_definitions(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

project (mess)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

find_package(Doxygen)
if (${DOXYGEN_FOUND})
	set(DOXYFILE_TO_CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYFILE_TO_CONFIGURE} ${DOXYFILE} @ONLY)
	
	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
endif(${DOXYGEN_FOUND})

include_directories(include)

find_package(GTest QUIET)
if(GTEST_FOUND)
	message(STATUS "Building Tests")
	add_executable(tests tests/test_broker.cpp)
	target_link_libraries(tests)

	add_executable(readme examples/minimal/readme.cpp)
	target_link_libraries(readme)

	add_executable(minimal examples/minimal/main.cpp examples/minimal/console.cpp examples/minimal/interpreter.cpp)
	target_link_libraries(minimal)

	if (CMAKE_VERSION VERSION_GREATER "3.9")
		cmake_policy(SET CMP0069 NEW)
		include(CheckIPOSupported)
		check_ipo_supported(RESULT supported OUTPUT error)

		if( supported )
			message(STATUS "IPO / LTO enabled")
			set(tests PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
		else()
			message(STATUS "IPO / LTO not supported: <${error}>")
		endif()
	endif()
endif()

install(DIRECTORY include/mess DESTINATION include)