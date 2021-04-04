cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Find deps
#################################
find_package(Python2 COMPONENTS Interpreter Development REQUIRED)
set(PYBIND11_FINDPYTHON ON)

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

target_link_libraries(pyfspp PRIVATE
	FSPP::fspp
)

# copy example
configure_file(version.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/version.py COPYONLY)

#################################
# Install
#################################
include(GNUInstallDirs)

install(TARGETS pyfspp
	LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT ${PROJECT_NAME}_Runtime
		NAMELINK_COMPONENT ${PROJECT_NAME}_Dev
	ARCHIVE
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT ${PROJECT_NAME}_Dev
)
