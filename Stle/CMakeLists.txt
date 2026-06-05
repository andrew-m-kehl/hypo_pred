cmake_minimum_required(VERSION 3.9)
project(Stle)


execute_process(COMMAND git describe --always --tags WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" OUTPUT_VARIABLE gitVersion ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if (gitVersion STREQUAL "")
  set(gitVersion unknown)
endif()

# TODO: The 'U' project should become a separate thing dynamically linked to the
# shared version of Stle.
file(GLOB_RECURSE stleSrc "*.cpp" "*.hpp")

# Static library.
add_library(stle-static STATIC ${stleSrc})

target_compile_features(stle-static PUBLIC cxx_std_14)

target_compile_definitions(stle-static PRIVATE -DSTLE_VERSION=\"${gitVersion}\")

target_include_directories(stle-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

#
add_library(stleHiddenPicObj OBJECT ${stleSrc})

set_property(TARGET stleHiddenPicObj PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET stleHiddenPicObj PROPERTY CXX_VISIBILITY_PRESET hidden)

target_compile_features(stleHiddenPicObj PUBLIC cxx_std_14)

target_compile_definitions(stleHiddenPicObj PRIVATE -DSTLE_VERSION=\"${gitVersion}\")

target_include_directories(stleHiddenPicObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_property(TARGET stleHiddenPicObj PROPERTY EXCLUDE_FROM_ALL ON)

# Shared library.
# Built if BUILD_SHARED_LIBS is ON.
add_library(stle-shared SHARED ${stleSrc})

set_property(TARGET stle-shared PROPERTY SOVERSION 1)
set_property(TARGET stle-shared PROPERTY OUTPUT_NAME stle)

target_compile_definitions(stle-shared PRIVATE -DSTLE_VERSION=\"${gitVersion}\")
target_compile_definitions(stle-shared PRIVATE -DSTLE_COMPILE_DLL)

target_compile_features(stle-shared PUBLIC cxx_std_14)

target_include_directories(stle-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_property(TARGET stle-shared PROPERTY EXCLUDE_FROM_ALL ${NO_BUILD_SHARED_LIBS})

# Interface library.
add_library(stle-interface INTERFACE)
target_compile_features(stle-interface INTERFACE cxx_std_14)
target_include_directories(stle-interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
