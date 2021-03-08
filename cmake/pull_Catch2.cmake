cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})
include_guard()

include(FetchContent)
#
# Catch2
#
find_package(Catch2 QUIET)

if(NOT Catch2_FOUND)
	message(STATUS "Standby, pulling Catch2...")
	FetchContent_Declare(c2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v2.13.1
	)

	FetchContent_GetProperties(c2)
	if(NOT c2_POPULATED)
		FetchContent_Populate(c2)
		add_subdirectory(${c2_SOURCE_DIR} ${c2_BINARY_DIR})
	endif()

	list(INSERT CMAKE_MODULE_PATH 0 "${c2_SOURCE_DIR}/contrib")
endif()

include(Catch)
include(ParseAndAddCatchTests)

