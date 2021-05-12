cmake_minimum_required(VERSION ${FSPP_CMAKE_VERSION})

#################################
# Define test targets
#################################
set(TARGET_NAME ${PROJECT_NAME}_tests)

add_executable(${TARGET_NAME}
	basic.cpp
)
target_link_libraries(${TARGET_NAME} PRIVATE
	${PROJECT_NAME}
	GTest::gtest_main
)
set_target_properties(${TARGET_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
)
gtest_discover_tests(${TARGET_NAME})
