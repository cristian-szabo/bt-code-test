cmake_minimum_required (VERSION 2.8.7)

project (BTCodeTest)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	message (FATAL_ERROR "In-tree builds are not supported; please run cmake in a separate build directory.")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()

add_subdirectory ("${CMAKE_SOURCE_DIR}/lib")
add_subdirectory ("${CMAKE_SOURCE_DIR}/app")
add_subdirectory ("${CMAKE_SOURCE_DIR}/test")
