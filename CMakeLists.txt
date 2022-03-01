cmake_minimum_required(VERSION 3.9.2)

project(mess VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(Warnings)

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} src/schedulers/std_thread.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads PRIVATE -fsanitize=undefined,address -fno-omit-frame-pointer)
target_set_warnings(${PROJECT_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC ${PROJECT_NAME} PRIVATE -fsanitize=undefined,address -fno-omit-frame-pointer)
target_set_warnings(main ENABLE ALL AS_ERROR ALL DISABLE Annoying)
set_target_properties(
	main
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

add_subdirectory(tests)