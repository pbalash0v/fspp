cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})


#################################
# Find deps
#################################
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

set_target_properties(pyfspp PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(pyfspp PUBLIC
	FSPP::fspp
)
