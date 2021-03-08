cmake_minimum_required(VERSION 3.16)

project(fspp
	VERSION 0.1.0
	LANGUAGES CXX
)

option(FSPP_BUILD_TESTS "Build tests" OFF)
option(FSPP_SANITIZED_TESTS "Build sanitized tests" OFF)
option(FSPP_STATIC_ANALYSIS "Use static analyzers" OFF)
option(FSPP_BUILD_EXAMPLES "Build example apps" OFF)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(SetupBuild)
include(pull_FreeSWITCH)
include(pull_fmt)

if(FSPP_STATIC_ANALYSIS)
	include(StaticAnalyzers)
endif()

add_subdirectory(src)

if (FSPP_BUILD_EXAMPLES)
	add_subdirectory(app)
endif()


if(FSPP_BUILD_TESTS)
	include(pull_Catch2)
	enable_testing()
	add_subdirectory(tests)
	if (FSPP_SANITIZED_TESTS)
		include(add_sanitizers)
	endif()
endif()



