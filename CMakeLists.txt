cmake_minimum_required (VERSION 2.8)

project (BTCodeTest)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	message (FATAL_ERROR "In-tree builds are not supported; please run cmake in a separate build directory.")
endif ()

add_subdirectory ("${CMAKE_SOURCE_DIR}/lib")
add_subdirectory ("${CMAKE_SOURCE_DIR}/app")
add_subdirectory ("${CMAKE_SOURCE_DIR}/test")
