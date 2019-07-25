cmake_minimum_required(VERSION 3.14)
project(zgui CXX)

add_library(zgui STATIC
  src/controls/button.cc
  src/controls/checkbox.cc
  src/controls/combobox.cc
  src/controls/groupbox.cc
  src/controls/key_bind.cc
  src/controls/listbox.cc
  src/controls/slider.cc
  src/controls/text.cc
  src/controls/text_input.cc
  src/controls/toggle_button.cc
  src/controls/window.cc)

set_target_properties(zgui PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17)

install(TARGETS zgui DESTINATION ${CMAKE_SOURCE_DIR}/lib)