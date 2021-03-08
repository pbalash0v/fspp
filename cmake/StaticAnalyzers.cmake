cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})
include_guard()

find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
	set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
else()
	message(SEND_ERROR "clang-tidy requested but executable not found")
endif()

