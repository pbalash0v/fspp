cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

# Some sanity check
# also to enable calling packaging w/ shorthanded defaults like:
# cmake --build . --target package
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CPACK_GENERATOR DEB)
else()
	message(FATAL_ERROR "Packaging for ${CMAKE_SYSTEM_NAME} not supported atm")
endif()


#################################
# Define CPACK vars
#################################
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_DESCRIPTION "FSPP library")
set(CPACK_PACKAGE_VENDOR "Pavel Balashov")
set(CPACK_PACKAGE_CONTACT "pbalashov@pm.me")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_MONOLITHIC_INSTALL OFF)
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)

set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/fspp")
#################################
# Define CPACK DEBIAN-specific vars
#################################
# auto discover shared libs dependency
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS YES)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")


set(CPACK_DEB_COMPONENT_INSTALL ON)

# set inter component dependencies if listed with CPACK_COMPONENT_<compName>_DEPENDS variables
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)

#
# Package and file naming (by component)
#
string(TOUPPER ${PROJECT_NAME}_Dev ${PROJECT_NAME}_Dev_uppercased)
string(REPLACE "_" "-" THIS_PACKAGE_NAME_PART ${PROJECT_NAME})

set(CPACK_DEBIAN_${${PROJECT_NAME}_Dev_uppercased}_PACKAGE_NAME ${THIS_PACKAGE_NAME_PART}-dev)
set(CPACK_DEBIAN_${${PROJECT_NAME}_Dev_uppercased}_FILE_NAME DEB-DEFAULT)


string(TOUPPER ${PROJECT_NAME}_Runtime ${PROJECT_NAME}_Runtime_uppercased)
set(CPACK_DEBIAN_${${PROJECT_NAME}_Runtime_uppercased}_PACKAGE_NAME ${THIS_PACKAGE_NAME_PART})
set(CPACK_DEBIAN_${${PROJECT_NAME}_Runtime_uppercased}_FILE_NAME DEB-DEFAULT)



#set(CPACK_DEBIAN_${${PROJECT_NAME}_Dev_uppercased}_PACKAGE_DEPENDS "libprotobuf-dev (>=3.6.1)")
set(CPACK_COMPONENT_${${PROJECT_NAME}_Dev_uppercased}_DEPENDS ${${PROJECT_NAME}_Runtime_uppercased})

# At this point where the CPack module is included,
# the CPackConfig.cmake file is written to the top of the build tree
include(CPack)


#################################
# Define components
#################################
# Notes:
# The effect of cpack_add_component() is to define a range of
# component-specific variables in the current scope.
# (each component's install content defined in corresponding target's CMakeLists.txt)

#
# Dev components 
#
cpack_add_component(${PROJECT_NAME}_Dev
	DISPLAY_NAME "Developer pre-requisites"
	DESCRIPTION "FSPP C++ library development headers and static library"
)

###
# Runtime components
###
cpack_add_component(${PROJECT_NAME}_Runtime
	DISPLAY_NAME "Runtime"
	DESCRIPTION "FSPP C++ library shared library"
)



####################################
# Debug, comment me out for clean output
####################################
if(OFF)
	get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
	message("CPACK_COMPONENTS_ALL: ${CPACK_COMPONENTS_ALL}")
	set(CPACK_DEBIAN_PACKAGE_DEBUG ON)
endif()
