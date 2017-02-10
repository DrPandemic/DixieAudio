cmake_minimum_required (VERSION 3.0)
project (test)

# Download and install GoogleTest
include(DownloadProject.cmake)
download_project(PROJ       googletest
	GIT_REPOSITORY      https://github.com/google/googletest.git
	GIT_TAG             master
	${UPDATE_DISCONNECTED_IF_AVAILABLE}
	)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})


# Trivial example using gtest and gmock
enable_testing()
SET(env{GTEST_COLOR} TRUE)
add_executable(tests main_test.cpp)
target_link_libraries(tests gtest)
add_test(NAME tests COMMAND tests)

