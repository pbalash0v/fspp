cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

add_subdirectory(lib)
if(FSPP_BUILD_EXAMPLES)
	add_subdirectory(app)
endif()
