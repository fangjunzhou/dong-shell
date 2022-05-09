cmake_minimum_required(VERSION 3.0.0)
project(DongShell VERSION 0.1.0)

MACRO(FILELIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(res "")
  FOREACH(child ${children})
    IF(NOT IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND res ${curdir}/${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${res})
ENDMACRO()

# Sync CMake config files.
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig/config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeConfig)

# CommandHandler
FILELIST(COMMAND_HANDLER ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler)
list(APPEND EXT_SOURCES ${COMMAND_HANDLER})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler)

# ShellDisplay
FILELIST(SHELL_DISPLAY ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandHandler)
list(APPEND EXT_SOURCES ${SHELL_DISPLAY})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ShellDisplay)

# eventpp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/packages/eventpp/include/eventpp)

include(CTest)
enable_testing()

# Main executable.
add_executable(DongShell src/main.cpp ${EXT_SOURCES})

# Tests
FILELIST(TEST_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/Tests)

# Catch test framework
find_package(Catch2 3 REQUIRED)

# These tests can use the Catch2-provided main
add_executable(Tests ${TEST_LIST} ${EXT_SOURCES})
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

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