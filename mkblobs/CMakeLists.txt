cmake_minimum_required(VERSION 3.9)

project(mkblobs)

file(GLOB mkblobsSrc "*.cpp")

add_executable(mkblobs ${mkblobsSrc})

target_compile_features(mkblobs PRIVATE cxx_std_14)

target_link_libraries(mkblobs stle-static)
