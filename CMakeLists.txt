cmake_minimum_required (VERSION 2.8.8)

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
# if (${DOXYGEN_FOUND})
# 	set(DOXYFILE_TO_CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
# 	set(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)

# 	configure_file(${DOXYFILE_TO_CONFIGURE} ${DOXYFILE} @ONLY)
	
# 	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
# endif(${DOXYGEN_FOUND})

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# add_subdirectory(external/safe)

include_directories(include)
include_directories(external/safe/include)

add_executable(tests tests/tests.cpp tests/test_state.cpp tests/test_resource.cpp tests/test_broker.cpp tests/test_workerpool.cpp tests/test_staticbroker.cpp)
target_link_libraries(tests gtest gmock ${CMAKE_THREAD_LIBS_INIT})