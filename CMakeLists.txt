cmake_minimum_required (VERSION 3.0)
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



# Download and install GoogleTest
include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})


# Trivial example using gtest and gmock
add_executable(tests test/main_test.cpp)
target_link_libraries(tests gtest)
add_test(NAME tests COMMAND tests)
add_custom_command(TARGET tests 
					POST_BUILD 
					COMMAND ./bin/tests)

# Try to find the PulseAudio library
#
# Once done this will define:
#
#  PULSEAUDIO_FOUND - system has the PulseAudio library
#  PULSEAUDIO_INCLUDE_DIR - the PulseAudio include directory
#  PULSEAUDIO_LIBRARY - the libraries needed to use PulseAudio
#
# Copyright (c) 2008, Matthias Kretz, <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (PULSEAUDIO_INCLUDE_DIR AND PULSEAUDIO_LIBRARY)
   # Already in cache, be silent
   set(PULSEAUDIO_FIND_QUIETLY TRUE)
endif (PULSEAUDIO_INCLUDE_DIR AND PULSEAUDIO_LIBRARY)

if (NOT WIN32)
   include(FindPkgConfig)
   pkg_check_modules(PULSEAUDIO libpulse)
   if(PULSEAUDIO_FOUND)
      set(PULSEAUDIO_LIBRARY ${PULSEAUDIO_LIBRARIES} CACHE FILEPATH "Path to the PulseAudio library")
      set(PULSEAUDIO_INCLUDE_DIR ${PULSEAUDIO_INCLUDEDIR} CACHE PATH "Path to the PulseAudio includes")
      #  PULSEAUDIO_DEFINITIONS - Compiler switches required for using PulseAudio
      #  set(PULSEAUDIO_DEFINITIONS ${PULSEAUDIO_CFLAGS})
   endif(PULSEAUDIO_FOUND)
endif (NOT WIN32)

if (NOT PULSEAUDIO_INCLUDE_DIR)
   FIND_PATH(PULSEAUDIO_INCLUDE_DIR pulse/pulseaudio.h)
endif (NOT PULSEAUDIO_INCLUDE_DIR)

if (NOT PULSEAUDIO_LIBRARY)
   FIND_LIBRARY(PULSEAUDIO_LIBRARY NAMES pulse)
endif (NOT PULSEAUDIO_LIBRARY)

if (PULSEAUDIO_INCLUDE_DIR AND PULSEAUDIO_LIBRARY)
   set(PULSEAUDIO_FOUND TRUE)
else (PULSEAUDIO_INCLUDE_DIR AND PULSEAUDIO_LIBRARY)
   set(PULSEAUDIO_FOUND FALSE)
endif (PULSEAUDIO_INCLUDE_DIR AND PULSEAUDIO_LIBRARY)

if (PULSEAUDIO_FOUND)
   if (NOT PULSEAUDIO_FIND_QUIETLY)
      message(STATUS "Found PulseAudio: ${PULSEAUDIO_LIBRARY}")
   endif (NOT PULSEAUDIO_FIND_QUIETLY)
else (PULSEAUDIO_FOUND)
   message(STATUS "Could NOT find PulseAudio")
endif (PULSEAUDIO_FOUND)

#mark_as_advanced(PULSEAUDIO_INCLUDE_DIR PULSEAUDIO_LIBRARY)