cmake_minimum_required(VERSION 3.21)

project(
	OSLAroused_SKSE
	VERSION 2.1.0
	LANGUAGES CXX
)

option(BUILD_SKYRIMSSE "Skyrim SSE build" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

macro(set_from_environment VARIABLE ENV_VAR)
	if(NOT DEFINED ${VARIABLE} AND DEFINED ENV{${ENV_VAR}})
		set(${VARIABLE} $ENV{${ENV_VAR}})
	endif()
endmacro()

if(BUILD_SKYRIMSSE)
	set_from_environment(Skyrim64Path SkyrimSE64Path)
	add_compile_definitions(SKYRIMSSE)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	set(SkyrimVersion "Skyrim SSE")
	set(CommonLibName "CommonLibSSE-po3")
	set(CommonLibPath "${ROOT_DIR}/external/CommonLibSSE-po3")
else()
	set_from_environment(Skyrim64Path Skyrim64Path)
	set(SkyrimVersion "Skyrim AE")
	set(CommonLibName "CommonLibSSE")
	set(CommonLibPath "${ROOT_DIR}/external/CommonLibSSE")
endif()

if(NOT DEFINED Skyrim64Path)
	message(FATAL_ERROR "Skyrim64Path is not set")
endif()


message(
	STATUS
	"Building for ${SkyrimVersion} at ${SkyrimPath}. Using ${CommonLibName} at ${CommonLibPath}"
)

option(COPY_OUTPUT "copy the output of build operations to the game directory" ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(src)
include(cmake/packaging.cmake)
