#################################
# Find deps
#################################

#detect FreeSWITCH
find_package(PkgConfig REQUIRED)
pkg_check_modules(freeswitch REQUIRED IMPORTED_TARGET freeswitch)

#if(c-ares_FOUND)
#	target_link_libraries(resip_static_resip INTERFACE PkgConfig::c-ares)
#endif()

