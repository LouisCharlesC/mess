cmake_minimum_required(VERSION 3.9.2)

project(mess VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(DocTarget)
include(Warnings)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# add_library(inclusivescanlib examples/inclusivescan/modules/source.cpp)
# target_include_directories(inclusivescanlib PUBLIC ${PROJECT_SOURCE_DIR}/examples/inclusivescan/modules)
# target_set_warnings(inclusivescanlib ENABLE ALL AS_ERROR ALL DISABLE Annoying)
# set_target_properties(
# 	inclusivescanlib
# 	PROPERTIES
# 		CXX_STANDARD 17
# 		CXX_EXTENSIONS NO
# )

# add_executable(inclusivescan examples/inclusivescan/apps/withmess/main.cpp)
# target_link_libraries(inclusivescan PRIVATE inclusivescanlib mess)
# target_set_warnings(inclusivescan ENABLE ALL AS_ERROR ALL DISABLE Annoying)
# set_target_properties(
# 	inclusivescan
# 	PROPERTIES
# 		CXX_STANDARD 17
# 		CXX_STANDARD_REQUIRED ON
# 		CXX_EXTENSIONS NO
# )

# add_executable(inclusivescanwomess examples/inclusivescan/apps/womess/main.cpp)
# target_link_libraries(inclusivescanwomess PRIVATE inclusivescanlib)
# target_set_warnings(inclusivescanwomess ENABLE ALL AS_ERROR ALL DISABLE Annoying)
# set_target_properties(
# 	inclusivescanwomess
# 	PROPERTIES
# 		CXX_STANDARD 17
# 		CXX_EXTENSIONS NO
# )

add_subdirectory(tests)