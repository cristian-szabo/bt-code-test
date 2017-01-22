project (CodeTestApp)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/inc")

file (GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp")
file (GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

source_group ("Include\\" FILES ${HEADER_FILES})
source_group ("Source\\" FILES ${SOURCE_FILES})

add_executable (${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries (${PROJECT_NAME} PUBLIC CodeTestLib)
