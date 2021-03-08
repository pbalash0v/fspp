cmake_minimum_required(VERSION 3.13)
include_guard()

find_package(fmt QUIET)

if(NOT fmt_FOUND)
	message(STATUS "Standby, pulling fmt...")
	FetchContent_Declare(fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG        6.2.1
		GIT_PROGRESS ON
	)
	FetchContent_MakeAvailable(fmt)
	FetchContent_GetProperties(fmt)
	if(NOT fmt_POPULATED)
	  FetchContent_Populate(fmt)
	  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
	endif()
endif()

