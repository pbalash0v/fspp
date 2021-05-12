cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Find deps
#################################


#################################
# Define targets
#################################
add_executable(app)
target_sources(app PRIVATE
	app.cpp
)
target_link_libraries(app PRIVATE
	Boost::boost
	FSPP::fspp
)

#[[
add_executable(pg)
target_sources(pg PRIVATE
	pg.cpp
)
set_target_properties(pg PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
target_compile_definitions(pg PRIVATE BOOST_DLL_USE_STD_FS)

target_link_libraries(pg PRIVATE
	Boost::boost
	std::filesystem
	${CMAKE_DL_LIBS}
	FSPP::fspp	
)
]]
