cmake_minimum_required(VERSION 3.13)

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

set_target_properties(app PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_link_libraries(app PRIVATE
	Boost::boost
	FSPP::fspp
)
