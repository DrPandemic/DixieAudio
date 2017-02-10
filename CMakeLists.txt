cmake_minimum_required (VERSION 3.0)
project (DixieAudio)
set(clang++ "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB DIXIEAUDIO_SOURCE
	src/*.cpp
	include/*.h
)

# Download and install GoogleTest
include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})


# Trivial example using gtest and gmock
enable_testing()
SET(env{GTEST_COLOR} TRUE)
add_executable(tests test/main_test.cpp)
target_link_libraries(tests gtest)
add_test(NAME tests COMMAND tests)

include(LibFindMacros.cmake)

libfind_pkg_check_modules(LibPulseSimple_PKGCONF libpulse-simple)

find_path(LibPulseSimple_INCLUDE_DIR
  NAMES pulse/simple.h
  HINTS ${LibPulseSimple_PKGCONF_INCLUDE_DIRS}
)

find_library(LibPulseSimple_LIBRARY
  NAMES pulse-simple
  HINTS ${LibPulseSimple_PKGCONF_LIBRARY_DIRS}
)

set(LibPulseSimple_PROCESS_INCLUDES LibPulseSimple_INCLUDE_DIR)
set(LibPulseSimple_PROCESS_LIBS LibPulseSimple_LIBRARY)
libfind_process(LibPulseSimple)

add_executable(DixieAudio ${DIXIEAUDIO_SOURCE})
target_link_libraries(DixieAudio ${LibPulseSimple_LIBRARIES})
