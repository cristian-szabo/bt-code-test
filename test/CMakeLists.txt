project (CodeTestCheck)

include (ExternalProject)

ExternalProject_Add(
  Bandit

  GIT_REPOSITORY "https://github.com/banditcpp/bandit.git"
  GIT_TAG "master"
  
  CMAKE_ARGS -DBANDIT_BUILD_SPECS=OFF -DBANDIT_RUN_SPECS=OFF
  
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/extern/bandit"
  
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(Bandit SOURCE_DIR)

include_directories ("${SOURCE_DIR}")
include_directories ("${CMAKE_SOURCE_DIR}/lib/inc")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/inc")

file (GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp")
file (GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

source_group ("Include\\" FILES ${HEADER_FILES})
source_group ("Source\\" FILES ${SOURCE_FILES})

add_executable (${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_dependencies (${PROJECT_NAME} Bandit)

target_link_libraries (${PROJECT_NAME} CodeTestLib)
