cmake_minimum_required(VERSION 3.14)
project(zgui CXX)

add_library(zgui STATIC src/graphics.hpp src/graphics.cpp)

set_target_properties(zgui PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17)

install(TARGETS zgui DESTINATION ${CMAKE_SOURCE_DIR}/lib)