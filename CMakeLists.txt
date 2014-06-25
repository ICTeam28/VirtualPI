# This file is part of the Team 28 Project
# Licensing information can be found in the LICENSE file
# (C) 2014 The Team 28 Authors. All rights reserved.
cmake_minimum_required(VERSION 2.8)
project(piemu)


# Dependencies
find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})


# Enable coverage in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools)
  include(CodeCoverage)
endif()


# C++ flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


# C++ sources
set(SOURCES
  memory.cc
  emulator.cc
  exception.cc
  arch/arm.cc
  arch/thumb.cc
)


# C++ headers
set(HEADERS
  common.h
  memory.h
  exception.h
  emulator.h
  arch/arm.h
  arch/thumb.h
)


# Libraries
set(LIBS
  m
  ${SDL_LIBRARY}
)


# Executable
add_executable(piemu main.cc ${SOURCES} ${HEADERS})
target_link_libraries(piemu ${LIBS})


# Test executable
if (CMAKE_BUILD_TYPE STREQUAL "Debug")

  # Set up a test wrapper
  add_executable(piemu_test test.cc ${SOURCES} ${HEADERS})
  target_link_libraries(piemu_test gcov ${LIBS})


  # Set up gcov
  setup_target_for_coverage(
    piemu_coverage
    piemu_test
    coverage
    ${CMAKE_SOURCE_DIR}/tests
  )
endif()
