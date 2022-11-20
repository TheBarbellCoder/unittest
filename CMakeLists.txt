cmake_minimum_required(VERSION 3.15)

project(
  Unittest
  VERSION 1.0
  DESCRIPTION "A minimal unit-testing framework for C++"
  LANGUAGES CXX)

option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compile commands for LSP" OFF)
option(FORK "Enable/disable forked processes for isolated testing" OFF)

add_library(unittest STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/unittest.cpp)

if(FORK AND UNIX)
  message("Forked testing enabled")
  target_compile_definitions(unittest PUBLIC FORK)
endif()

target_include_directories(
  unittest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
                  $<INSTALL_INTERFACE:include/unittest>)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  message("Compile commands export enabled")
endif()

string(COMPARE EQUAL ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
               IS_SOURCE_DIR)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND ${IS_SOURCE_DIR})
  add_custom_target(
    compile_commands ALL
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            compile_commands.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating compile_commands.json ...")
endif()
