cmake_minimum_required(VERSION 3.9)

project(stle-cli)

file(GLOB stleCliSrc "*.cpp" "*.hpp")

execute_process(COMMAND git describe --always --tags WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" OUTPUT_VARIABLE gitVersion ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if (gitVersion STREQUAL "")
  set(gitVersion unknown)
endif()

file(GLOB blobsSrc "*.txt")
list(FILTER blobsSrc EXCLUDE REGEX ".*CMakeLists.txt$")
add_custom_command(OUTPUT Blobs.cpp Blobs.hpp COMMAND mkblobs ${blobsSrc} DEPENDS ${blobsSrc})

add_executable(stle-cli ${stleCliSrc} Blobs.cpp Blobs.hpp)

set_property(TARGET stle-cli PROPERTY OUTPUT_NAME stle)

target_compile_features(stle-cli PRIVATE cxx_std_14)

# To pick up generated Blobs.hpp.
target_include_directories(stle-cli PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(stle-cli PRIVATE -DSTLE_CLI_VERSION=\"${gitVersion}\")

target_link_libraries(stle-cli stle-static)
