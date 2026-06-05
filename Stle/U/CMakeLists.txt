cmake_minimum_required(VERSION 3.9)

project(Util)

file(GLOB UtilSrc "*.cpp" "*.hpp")

add_library(Util STATIC ${UtilSrc})

target_compile_features(Util PUBLIC cxx_std_14)

set_property(TARGET Util PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

target_include_directories(Util INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")
