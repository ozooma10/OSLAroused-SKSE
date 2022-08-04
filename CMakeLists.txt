cmake_minimum_required(VERSION 3.22)

project(
	OSLAroused_SKSE
	VERSION 2.2.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add options for specifying the version
# Only one version may be provided at a time
option(SKYRIM_AE "Compile plugin for Skyrim AE" OFF)
option(SKYRIM_SE "Compile plugin for Skyrim SE" OFF)
option(SKYRIM_VR "Compile plugin for Skyrim VR" OFF)

# Exit if no Skyrim version was specified
if(NOT ${SKYRIM_AE} AND NOT ${SKYRIM_SE} AND NOT ${SKYRIM_VR})
	message(FATAL_ERROR "Run cmake using one of the build/ .bat files for building Skyrim")
	return()
endif()

# Add support for #ifdef for SKYRIM_AE, SKYRIM_SE, and SKYRIM_VR
if(${SKYRIM_AE})
	message(INFO "Building CMake setup for Skyrim AE: ${SKYRIM_AE}")
	add_definitions(-DSKYRIM_AE)
	set(SKYRIM_SUPPORT_AE true)
	set(SKYRIM_VERSION AE)
elseif(${SKYRIM_SE})
	message(INFO "Building CMake setup for Skyrim SE: ${SKYRIM_SE}")
	add_definitions(-DSKYRIM_SE)
	set(SKYRIM_VERSION SE)
elseif(${SKYRIM_VR})
	message(INFO "Building CMake setup for Skyrim VR: ${SKYRIM_VR}")
	add_definitions(-DSKYRIM_VR)
	set(SKYRIM_VERSION VR)
endif()

# Enable testing
#enable_testing()

# Process the .in templates in the cmake/ folder for Plugin.h and version.rc
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Process the CMakeLists.txt in the src/ folder
add_subdirectory(src)
#add_subdirectory(test)
