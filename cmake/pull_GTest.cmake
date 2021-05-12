cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})
include_guard()
include(FetchContent)

set(INSTALL_GTEST OFF CACHE STRING "" FORCE)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
	message(STATUS "Pulling GTest...")
	FetchContent_Declare(googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG f5e592d8ee5ffb1d9af5be7f715ce3576b8bf9c4
		GIT_PROGRESS ON
		GIT_SHALLOW ON		
	)
	FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
