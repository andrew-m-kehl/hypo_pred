cmake_minimum_required(VERSION 3.9)

set(STLE_SET_BUILD_TYPE ON CACHE BOOL "Set default build type to Release (Default: ON).")
if (STLE_SET_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (Default: Release).")
else()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "Link-time optimization (Default: ON except for Debug).")
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON CACHE BOOL "Link-time optimization (Default: ON except for Debug).")
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared versions of libraries (Default: OFF).")
if (BUILD_SHARED_LIBS)
    set (NO_BUILD_SHARED_LIBS OFF)
else()
    set (NO_BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_SKIP_RPATH ON CACHE BOOL "Do not add RPATH/RUNPATH to executables.")

set(CMAKE_DEBUG_POSTFIX d)

project(StlEval)

set(BUILD_SHARED_LIBS_OLD ${BUILD_SHARED_LIBS})
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_OLD ${CMAKE_INTERPROCEDURAL_OPTIMIZATION})

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gtest-lib)
add_subdirectory(googletest EXCLUDE_FROM_ALL)

set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_OLD} CACHE BOOL "" FORCE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${CMAKE_INTERPROCEDURAL_OPTIMIZATION_OLD} CACHE BOOL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()
    if(CMAKE_CXX_FLAGS MATCHES "/Wall")
        string(REGEX REPLACE "/Wall" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")    
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_subdirectory(Stle)
add_subdirectory(CStle)
add_subdirectory(tests)
add_subdirectory(mkblobs)
add_subdirectory(stle-cli)
add_subdirectory(mksignal)

add_executable(ekg_demo ekg_demo.cpp)
target_link_libraries(ekg_demo stle-static)

