cmake_minimum_required(VERSION 3.0)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
SET(env{CTEST_OUTPUT_ON_FAILURE} TRUE)
SET(env{GTEST_COLOR} TRUE)

# Boost
find_package(Boost 1.58 REQUIRED COMPONENTS system thread date_time)
include_directories(${Boost_INCLUDE_DIR})

set(TEST_LIBRARIES ${GTEST_BOTH_LIBRARIES} dixieaudio ${CMAKE_THREAD_LIBS_INIT})

# Link runTests with what we want to test and the GTest and pthread library
include_directories(${dixieaudio_INCLUDE_DIR})
include_directories(${dixieaudio_SOURCE_DIR})

add_executable(tests main_test.cpp)
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES} ${TEST_LIBRARY} ${Boost_LIBRARIES} -lpthread -lgtest -lgmock)
