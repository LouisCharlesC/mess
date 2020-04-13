cmake_minimum_required(VERSION 3.9.2)

project(mess VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(DocTarget)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

add_subdirectory(tests)