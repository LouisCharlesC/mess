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

add_executable(helloworld examples/helloworld.cpp)
add_executable(helloworldwomess examples/womess/helloworld.cpp)
add_executable(hellofile examples/hellofile.cpp)

install(DIRECTORY include/mess DESTINATION include)