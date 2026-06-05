cmake_minimum_required(VERSION 3.9)

project(tests)

file(GLOB testsSrc "*.cpp" "*.hpp")

# Objects for tests.
# Built once, without USE_CSTLE_DLL (since __declspec(dllimport) is not strictly required) and then
# linked into 'tests' and 'tests-shared'.
# Not built if not used.
add_library(testsObj OBJECT ${testsSrc})

target_compile_features(testsObj PRIVATE cxx_std_14)
# Getting header paths from used libraries.
# TODO: When switching to CMake 3.12, use target_link_libraries instead.
target_include_directories(testsObj PRIVATE
  $<TARGET_PROPERTY:stle-interface,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:cstle-interface,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:gtest,INTERFACE_INCLUDE_DIRECTORIES>)

set_property(TARGET testsObj PROPERTY EXCLUDE_FROM_ALL ON)

# Tests statically linked to Stle libraries.
add_executable(tests $<TARGET_OBJECTS:testsObj>)
target_link_libraries(tests stle-static cstle-static gtest)

# Tests dynamically linked to Stle libraries.
# Built if BUILD_SHARED_LIBS is ON.
add_executable(tests-shared $<TARGET_OBJECTS:testsObj>)
target_link_libraries(tests-shared stle-shared cstle-shared gtest)

set_property(TARGET tests-shared PROPERTY EXCLUDE_FROM_ALL ${NO_BUILD_SHARED_LIBS})
