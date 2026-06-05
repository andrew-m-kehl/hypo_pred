cmake_minimum_required(VERSION 3.9)

project(CStle)

file(GLOB cstleSrc "*.cpp" "*.h")

execute_process(COMMAND git describe --always --tags WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" OUTPUT_VARIABLE gitVersion ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if (gitVersion STREQUAL "")
  set(gitVersion unknown)
endif()

# Static library.
add_library(cstle-static STATIC ${cstleSrc})

set_property(TARGET cstle-static PROPERTY OUTPUT_NAME cstle-static)

target_compile_features(cstle-static PRIVATE cxx_std_14)

target_compile_definitions(cstle-static PRIVATE -DCSTLE_VERSION=\"${gitVersion}\")

target_include_directories(cstle-static INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(cstle-static PRIVATE stle-static)

# Shared library.
# In Windows, we can just link to 'stle-static', which will not export Stle C++ 
# symbols.
# In Linux, we want to link to the objects compiled with PIC with hidden
# visibility.
if (WIN32)
    add_library(cstle-shared SHARED ${cstleSrc})
    target_link_libraries(cstle-shared PRIVATE stle-static)
else()
    add_library(cstle-shared SHARED ${cstleSrc} $<TARGET_OBJECTS:stleHiddenPicObj>)
    set_property(TARGET cstle-shared PROPERTY CXX_VISIBILITY_PRESET hidden)
    target_link_libraries(cstle-shared PRIVATE stle-interface)
endif()

set_property(TARGET cstle-shared PROPERTY OUTPUT_NAME cstle)
set_property(TARGET cstle-shared PROPERTY SOVERSION 1)

target_compile_features(cstle-shared PRIVATE cxx_std_14)

target_compile_definitions(cstle-shared PRIVATE -DCSTLE_VERSION=\"${gitVersion}\")
target_compile_definitions(cstle-shared PRIVATE -DCSTLE_COMPILE_DLL)

target_include_directories(cstle-shared INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_property(TARGET cstle-shared PROPERTY EXCLUDE_FROM_ALL ${NO_BUILD_SHARED_LIBS})

# Interface library.
add_library(cstle-interface INTERFACE)
target_include_directories(cstle-interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
