cmake_minimum_required(VERSION 3.14)
project(zgui_sfml_graphics CXX)

file(GLOB_RECURSE SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.hpp)

set(GCC_COVERAGE_COMPILE_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/")
find_package(SFML REQUIRED system window graphics network audio)

include_directories(${SFML_INCLUDE_DIR})
add_executable(zgui_sfml_graphics ${SRC})
target_link_libraries(zgui_sfml_graphics ${SFML_LIBRARIES})

set_target_properties(zgui_sfml_graphics PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17)