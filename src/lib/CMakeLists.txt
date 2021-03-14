cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Find deps
#################################

#################################
# Define targets
#################################
add_library(${PROJECT_NAME})
add_library(FSPP::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}.cpp
	${PROJECT_NAME}_impl.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	POSITION_INDEPENDENT_CODE ON
	ENABLE_EXPORTS ON
)
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
target_link_libraries(${PROJECT_NAME} PRIVATE
	Boost::boost
	$<BUILD_INTERFACE:fmt::fmt-header-only>	
	${CMAKE_DL_LIBS}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_DLL_USE_STD_FS)
target_link_libraries(${PROJECT_NAME} PUBLIC
	std::filesystem
	FS::freeswitch_static
)
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
