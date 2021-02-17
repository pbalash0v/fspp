cmake_minimum_required(VERSION 3.13)

#################################
# Define targets
#################################
add_library(${PROJECT_NAME})
add_library(FSPP::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	POSITION_INDEPENDENT_CODE ON
)
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
target_link_libraries(${PROJECT_NAME} PRIVATE
	Boost::boost
	PkgConfig::freeswitch
)
