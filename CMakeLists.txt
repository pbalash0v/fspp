cmake_minimum_required(VERSION 3.16)

project(fspp
	VERSION 0.2.1
	LANGUAGES CXX
)

option(FSPP_BUILD_TESTS "Build tests" OFF)
option(FSPP_SANITIZED_TESTS "Build sanitized tests" OFF)
option(FSPP_STATIC_ANALYSIS "Use static analyzers" OFF)
option(FSPP_BUILD_EXAMPLES "Build example apps" ON)
option(FSPP_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(SetupBuild)
include(pull_FreeSWITCH)
include(pull_fmt)


if(FSPP_STATIC_ANALYSIS)
	include(StaticAnalyzers)
endif()

add_subdirectory(src)

if(FSPP_BUILD_TESTS)
	include(pull_Catch2)
	enable_testing()
	add_subdirectory(tests)
	if (FSPP_SANITIZED_TESTS)
		include(add_sanitizers)
	endif()
endif()


# packaging
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	add_subdirectory(packaging)
endif()
