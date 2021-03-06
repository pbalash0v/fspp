cmake_minimum_required(VERSION 3.16)
include_guard()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
		message(FATAL_ERROR "In-source builds are not allowed.")
	endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

	# Set build tree output directories for convenience
	if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin)
	endif()

	# Be specific by assuming default build type is "debug" if no build type was explicitly set
	if(CMAKE_BUILD_TYPE STREQUAL "")
		set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type configuration" FORCE)
	endif()
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

###########################################
# Anything global and common to all targets
###########################################
set(FSPP_CMAKE_VERSION "3.16")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.74.0 REQUIRED COMPONENTS)# system filesystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ENABLE_EXPORTS ON) # !!! THIS SHOULD BE SET FOR ANY TARGET THAT USES FSPP::fspp !!!

find_package(Filesystem REQUIRED)

include(ExternalProject)
include(FetchContent)
