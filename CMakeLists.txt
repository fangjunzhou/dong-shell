cmake_minimum_required(VERSION 3.0.0)
project(DongShell VERSION 0.1.0)

# Source Code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Sync CMake config files.
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig/config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig)

# CommandHandler
set(COMMAND_HANDLER
${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler/CommandHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler/CommandHandler.hpp
)

include(CTest)
enable_testing()

# Main executable.
add_executable(DongShell src/main.cpp ${COMMAND_HANDLER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
