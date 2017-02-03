cmake_minimum_required (VERSION 2.6)
project (DikieAudio)
set(clang++ "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB DIXIEAUDIO_SOURCE
	src/*.cpp
	include/*.h
)

add_executable(DixieAudio ${DIXIEAUDIO_SOURCE})