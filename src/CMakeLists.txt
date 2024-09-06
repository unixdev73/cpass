configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/version.hpp @ONLY)

if(NOT CMAKE_INSTALL_RPATH)
	if(NOT "$ENV{CMAKE_INSTALL_RPATH}" STREQUAL "")
		set(CMAKE_INSTALL_RPATH "$ENV{CMAKE_INSTALL_RPATH}")
		message(STATUS "CMAKE_INSTALL_RPATH = \"$ENV{CMAKE_INSTALL_RPATH}\"")
	endif()
endif()

if(NOT CMAKE_BUILD_RPATH)
	if(NOT "$ENV{CMAKE_BUILD_RPATH}" STREQUAL "")
		set(CMAKE_BUILD_RPATH "$ENV{CMAKE_BUILD_RPATH}")
		message(STATUS "CMAKE_BUILD_RPATH = \"$ENV{CMAKE_BUILD_RPATH}\"")
	endif()
endif()

add_executable(cpass main.cpp)
target_include_directories(cpass PRIVATE ${CMAKE_BINARY_DIR})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "CMAKE_BUILD_TYPE = Debug")
	target_compile_options(cpass PUBLIC
		-Wall
		-Wextra
		-Wpedantic
		-fdiagnostics-color=always
		-O0
		-g
		-fsanitize=leak
	)
else()
	message(STATUS "CMAKE_BUILD_TYPE = Release")
	target_compile_options(cpass PUBLIC
		-Wall
		-Wextra
		-Wpedantic
		-fdiagnostics-color=always
		-O3
	)
endif()

install(TARGETS cpass DESTINATION bin)
