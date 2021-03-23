cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Find deps
#################################
find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(CMAKE_CXX_VISIBILITY_PRESET "default")

message(STATUS "Standby, pulling pybind11...")
FetchContent_Declare(pybind
	GIT_REPOSITORY https://github.com/pybind/pybind11.git
	GIT_TAG origin/master
	GIT_PROGRESS ON
)
FetchContent_MakeAvailable(pybind)

#################################
# Define targets
#################################
pybind11_add_module(pyfspp module.cpp)
target_compile_options(pyfspp PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
set_target_properties(pyfspp PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	POSITION_INDEPENDENT_CODE ON
	ENABLE_EXPORTS ON
)

target_link_libraries(pyfspp PUBLIC
	FSPP::fspp
)

# copy example
configure_file(version.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

