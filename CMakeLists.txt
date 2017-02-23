cmake_minimum_required (VERSION 3.0)
project (DixieAudio)
set(clang++ "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_CXX_STANDARD 14) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(dixieaudio_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(dixieaudio_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

find_package(Boost 1.58 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB DIXIEAUDIO_SOURCE
	src/*.cpp
	include/*.h
  application/main.cpp
)

add_subdirectory(test)

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
target_link_libraries(DixieAudio ${LibPulseSimple_LIBRARIES} ${Boost_LIBRARIES})

