cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Define targets
#################################
add_library(${PROJECT_NAME})
add_library(FSPP::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}.cpp
	${PROJECT_NAME}_impl.cpp
	fs_cfg.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
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

#################################
# Install
#################################
#[[
install(TARGETS ${PROJECT_NAME}
	LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT ${PROJECT_NAME}_Runtime
		NAMELINK_COMPONENT ${PROJECT_NAME}_Dev
	ARCHIVE
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT ${PROJECT_NAME}_Dev
)
]]

