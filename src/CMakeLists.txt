cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

add_subdirectory(lib)

if(FSPP_BUILD_EXAMPLES)
	add_subdirectory(app)
endif()

if(FSPP_BUILD_PYTHON_BINDINGS)
	add_subdirectory(python)
endif()
