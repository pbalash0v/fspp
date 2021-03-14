cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})
include_guard()

#############
# Find deps #
#############
find_package(OpenSSL 1.1.1 REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON) # Without this flag CMake may resort to just '-lpthread'
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(THIRD_PTY_LIBS REQUIRED IMPORTED_TARGET speexdsp speex libpcre libcurl libtpl uuid)

#####################
# Pull FS and stuff #
#####################
set(BUILD_ARTIFACTS_DIR ${CMAKE_BINARY_DIR}/build_artifacts)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(AUTOTOOLS_FLAGS "CFLAGS=-ggdb3 -O0")
else()
	set(AUTOTOOLS_FLAGS CPPFLAGS=-DNDEBUG CFLAGS=-O2)
endif()

ExternalProject_Add(spandsp
	GIT_REPOSITORY https://github.com/freeswitch/spandsp.git
	GIT_TAG origin/master
	GIT_SHALLOW ON
	GIT_PROGRESS ON

	CONFIGURE_COMMAND cd <SOURCE_DIR> && ./bootstrap.sh && ./configure --prefix=${BUILD_ARTIFACTS_DIR}
	BUILD_COMMAND cd <SOURCE_DIR> && make
	INSTALL_COMMAND cd <SOURCE_DIR> && make install
	UPDATE_COMMAND ""
	BUILD_ALWAYS OFF
)

ExternalProject_Add(sofia_sip
	GIT_REPOSITORY https://github.com/freeswitch/sofia-sip.git
	GIT_TAG origin/master
	GIT_SHALLOW ON
	GIT_PROGRESS ON

	CONFIGURE_COMMAND cd <SOURCE_DIR> && ./bootstrap.sh && ./configure --prefix=${BUILD_ARTIFACTS_DIR}
	BUILD_COMMAND cd <SOURCE_DIR> && make
	INSTALL_COMMAND cd <SOURCE_DIR> && make install
	UPDATE_COMMAND ""
	BUILD_ALWAYS OFF
)


set(FS_CFG_CMD
	PKG_CONFIG_PATH=${BUILD_ARTIFACTS_DIR}/lib/pkgconfig ./configure --prefix=${BUILD_ARTIFACTS_DIR} --disable-libyuv --disable-libvpx --disable-srtp --disable-core-libedit-support --without-freetype --enable-core-odbc-support=no
)
set(FS_PATCH_DIR ${CMAKE_CURRENT_LIST_DIR}/FS_build_conf_patches)
set(FS_PATCH_CMD
	cp ${FS_PATCH_DIR}/modules.conf <SOURCE_DIR> && 
	cp ${FS_PATCH_DIR}/mod_commands_Makefile.am <SOURCE_DIR>/src/mod/applications/mod_commands/Makefile.am && 
	cp ${FS_PATCH_DIR}/mod_console_Makefile.am <SOURCE_DIR>/src/mod/loggers/mod_console/Makefile.am && 
	cp ${FS_PATCH_DIR}/mod_logfile_Makefile.am <SOURCE_DIR>/src/mod/loggers/mod_logfile/Makefile.am &&
	cp ${FS_PATCH_DIR}/mod_event_socket_Makefile.am <SOURCE_DIR>/src/mod/event_handlers/mod_event_socket/Makefile.am
)

ExternalProject_Add(freeswitch
	GIT_REPOSITORY https://github.com/signalwire/freeswitch
	GIT_TAG origin/master
	GIT_SHALLOW ON
	GIT_PROGRESS ON
	
	PATCH_COMMAND ${FS_PATCH_CMD}
	CONFIGURE_COMMAND cd <SOURCE_DIR> && ./bootstrap.sh && ${FS_CFG_CMD}
	BUILD_COMMAND cd <SOURCE_DIR> && make
	INSTALL_COMMAND cd <SOURCE_DIR> && make install
	UPDATE_COMMAND ""
	BUILD_ALWAYS OFF
)
ExternalProject_Get_property(freeswitch SOURCE_DIR)
ExternalProject_Add_StepDependencies(freeswitch configure spandsp sofia_sip)


#detect FreeSWITCH
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(freeswitch REQUIRED IMPORTED_TARGET freeswitch)
#if(freeswitch_FOUND)
#	target_link_libraries(resip_static_resip INTERFACE PkgConfig::freeswitch)
#endif()


#########################################################
# Define imported static lib from compiled FS component #
#########################################################
macro(fs_mod_2_lib STATIC_LIB_NAME MOD_LIB_NAME MOD_LIB_PATH)
	add_library(${STATIC_LIB_NAME} STATIC IMPORTED GLOBAL)

	file(MAKE_DIRECTORY ${MOD_LIB_PATH})
	file(TOUCH ${MOD_LIB_PATH}/${MOD_LIB_NAME})
	
	find_library(${STATIC_LIB_NAME}_LIBRARY
		REQUIRED
		NAMES ${MOD_LIB_NAME}
		HINTS ${MOD_LIB_PATH}
	)
	target_link_libraries(${STATIC_LIB_NAME} INTERFACE
		freeswitch_static
	)
	set_target_properties(${STATIC_LIB_NAME} PROPERTIES
		IMPORTED_LOCATION ${${STATIC_LIB_NAME}_LIBRARY}
	)	
endmacro()
########################
# static libfreeswitch #
########################
# just "decalring" here to be used as a module dependancy
add_library(freeswitch_static STATIC IMPORTED GLOBAL)
# Hack to make it work, otherwise INTERFACE_INCLUDE_DIRECTORIES etc will not be propagated
file(MAKE_DIRECTORY ${BUILD_ARTIFACTS_DIR}/include/freeswitch)
file(MAKE_DIRECTORY ${BUILD_ARTIFACTS_DIR}/lib)
file(TOUCH ${BUILD_ARTIFACTS_DIR}/lib/libfreeswitch.a)

#######################
# libs, FS depends on #
#######################
fs_mod_2_lib(apr_static libapr-1.a ${SOURCE_DIR}/libs/apr/.libs)
fs_mod_2_lib(apr_util_static libaprutil-1.a ${SOURCE_DIR}/libs/apr-util/.libs)
fs_mod_2_lib(sofia_sip_static libsofia-sip-ua.a ${BUILD_ARTIFACTS_DIR}/lib)
fs_mod_2_lib(spandsp_static libspandsp.a ${BUILD_ARTIFACTS_DIR}/lib)

###################################
# FS modules built as static libs #
###################################
fs_mod_2_lib(mod_sofia_static libsofiamod.a ${SOURCE_DIR}/src/mod/endpoints/mod_sofia/.libs)
fs_mod_2_lib(mod_event_socket_static libmod_event_socket.a ${SOURCE_DIR}/src/mod/event_handlers/mod_event_socket/.libs)
fs_mod_2_lib(mod_commands_static libmod_commands.a ${SOURCE_DIR}/src/mod/applications/mod_commands/.libs)
fs_mod_2_lib(mod_logfile_static libmod_logfile.a ${SOURCE_DIR}/src/mod/loggers/mod_logfile/.libs)
fs_mod_2_lib(mod_console_static libmod_console.a ${SOURCE_DIR}/src/mod/loggers/mod_console/.libs)


# now "define" freeswitch_static
find_library(FREESWITCH_STATIC_LIBRARY
	REQUIRED
	NAMES libfreeswitch.a
	HINTS ${BUILD_ARTIFACTS_DIR}/lib
)
set_target_properties(freeswitch_static PROPERTIES
	IMPORTED_LOCATION ${FREESWITCH_STATIC_LIBRARY}
)

target_include_directories(freeswitch_static SYSTEM INTERFACE ${BUILD_ARTIFACTS_DIR}/include/freeswitch)
target_link_libraries(freeswitch_static INTERFACE
	mod_console_static
	mod_logfile_static
	mod_commands_static
	mod_event_socket_static
	mod_sofia_static
	sofia_sip_static
	spandsp_static
	apr_util_static
	apr_static
	Threads::Threads
	OpenSSL::SSL
	OpenSSL::Crypto
	SQLite::SQLite3
	ZLIB::ZLIB
	PkgConfig::THIRD_PTY_LIBS
)
add_library(FS::freeswitch_static ALIAS freeswitch_static)

