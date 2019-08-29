cmake_minimum_required(VERSION 3.14)
project(zgui_sfml_graphics CXX)

add_executable(zgui_sfml_graphics
        ../../../src/graphics.hpp
        ../../../src/graphics.cpp
        ../../../src/input.hpp
        sfml_input.hpp
        sfml_input.cpp
        sfml_graphics.hpp
        sfml_graphics.cpp
        main.cpp)

set(GCC_COVERAGE_COMPILE_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    message("FOUND")
    include_directories(${SFML_INCLUDE_DIR})
    message(STATUS "Libs: ${SFML_LIBRARIES}")
    target_link_libraries(zgui_sfml_graphics ${SFML_LIBRARIES})
endif()

set_target_properties(zgui_sfml_graphics PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17)
