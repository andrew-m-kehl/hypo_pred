cmake_minimum_required(VERSION 3.9)

project(mksignal)

file(GLOB mksignalSrc "*.cpp")

add_executable(mksignal ${mksignalSrc})

target_compile_features(mksignal PRIVATE cxx_std_14)

target_link_libraries(mksignal stle-static)
