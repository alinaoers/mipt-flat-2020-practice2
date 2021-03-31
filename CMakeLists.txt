cmake_minimum_required(VERSION 2.8)
project(earley_parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(GTest REQUIRED)

if(NOT GTest_FOUND)
  message(SEND_ERROR "Failed to find gtest library")
  return()
else()
  include_directories(${GTEST_INCLUDE_DIRS})
endif()

add_library(mainlib STATIC Earley.cpp)

add_executable(main main.cpp)
target_link_libraries(main mainlib)

add_executable(maintest test.cpp)
target_link_libraries(maintest mainlib)
target_link_libraries(maintest ${GTEST_LIBRARIES} pthread)

enable_testing()
