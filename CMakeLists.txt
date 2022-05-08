cmake_minimum_required(VERSION 3.0.0)
project(DongShell VERSION 0.1.0)

# Sync CMake config files.
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig/config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig)

# CommandHandler
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler)

# ShellDisplay
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/ShellDisplay)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ShellDisplay)

# eventpp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/packages/eventpp/include/eventpp)

include(CTest)
enable_testing()

# Main executable.
add_executable(DongShell src/main.cpp)
target_link_libraries(DongShell PUBLIC CommandHandler)
target_link_libraries(DongShell PUBLIC ShellDisplay)

# Catch test framework
find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests src/test.cpp)
target_link_libraries(tests PUBLIC CommandHandler)
target_link_libraries(tests PUBLIC ShellDisplay)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# eventpp
find_package(eventpp)
if(eventpp_FOUND)
    target_link_libraries(DongShell PUBLIC eventpp::eventpp)
else(eventpp_FOUND)
    message(FATAL_ERROR "eventpp library is not found")
endif(eventpp_FOUND)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)