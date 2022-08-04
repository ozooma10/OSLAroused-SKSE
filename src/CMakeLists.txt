set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCE_DIR "${ROOT_DIR}/src")
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h" 
	"${SOURCE_DIR}/Papyrus/*.cpp" "${SOURCE_DIR}/Papyrus/*.h"
	"${SOURCE_DIR}/Managers/*.cpp" "${SOURCE_DIR}/Managers/*.h"
	"${SOURCE_DIR}/Utilities/*.cpp" "${SOURCE_DIR}/Utilities/*.h"
	"${SOURCE_DIR}/Integrations/*.cpp" "${SOURCE_DIR}/Integrations/*.h"
)

source_group(TREE "${ROOT_DIR}" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)
source_group("src" FILES "${VERSION_HEADER}")

configure_file(
	"${ROOT_DIR}/cmake/version.rc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	@ONLY
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	"${ROOT_DIR}/.clang-format"
	"${ROOT_DIR}/.editorconfig"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_23
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif()

# Default output directory for compiled files
target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/src"
		"${SOURCE_DIR}"
)

# CommonLib
if(${SKYRIM_AE})
	add_subdirectory("${ROOT_DIR}/CommonLib/AE" CommonLibSSE EXCLUDE_FROM_ALL)
    target_link_libraries(
        "${PROJECT_NAME}"
        PUBLIC
            CommonLibSSE::CommonLibSSE
    )
elseif(${SKYRIM_SE})
	add_subdirectory("${ROOT_DIR}/CommonLib/SE" CommonLibSSE EXCLUDE_FROM_ALL)
    target_link_libraries(
        "${PROJECT_NAME}"
        PUBLIC
            CommonLibSSE::CommonLibSSE
    )
elseif(${SKYRIM_VR})
	add_subdirectory("${ROOT_DIR}/CommonLib/VR" CommonLibVR EXCLUDE_FROM_ALL)
    target_link_libraries(
        "${PROJECT_NAME}"
        PUBLIC
            CommonLibVR::CommonLibVR
    )
endif()

# Include spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries("${PROJECT_NAME}" PUBLIC spdlog::spdlog)

# Precompiled Headers (PCH)
target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/PCH.h"
)

# Get output directory based on configured version of Skyrim
if(${SKYRIM_AE})
	if(DEFINED ENV{SKSE_SKYRIM_AE_DEV})
		set(MOD_FOLDER $ENV{SKSE_SKYRIM_AE_DEV})
	elseif(DEFINED ENV{SKYRIM_MODS})
		set(MOD_FOLDER "$ENV{SKYRIM_MODS}/${PROJECT_NAME} - ${SKYRIM_VERSION}")
	else()
		set(MOD_FOLDER "${ROOT_DIR}/output/${SKYRIM_VERSION}")
	endif()
elseif(${SKYRIM_SE})
	if(DEFINED ENV{SKSE_SKYRIM_SE_DEV})
		set(MOD_FOLDER $ENV{SKSE_SKYRIM_SE_DEV})
	elseif(DEFINED ENV{SKYRIM_MODS})
		set(MOD_FOLDER "$ENV{SKYRIM_MODS}/${PROJECT_NAME} - ${SKYRIM_VERSION}")
	else()
		set(MOD_FOLDER "${ROOT_DIR}/output/${SKYRIM_VERSION}")
	endif()
elseif(${SKYRIM_VR})
	if(DEFINED ENV{SKSE_SKYRIM_VR_DEV})
		set(MOD_FOLDER $ENV{SKSE_SKYRIM_VR_DEV})
	elseif(DEFINED ENV{SKYRIM_MODS})
		set(MOD_FOLDER "$ENV{SKYRIM_MODS}/${PROJECT_NAME} - ${SKYRIM_VERSION}")
	else()
		set(MOD_FOLDER "${ROOT_DIR}/output/${SKYRIM_VERSION}")
	endif()
endif()

set(DLL_FOLDER "${MOD_FOLDER}/SKSE/Plugins")

# Create output directory if it does not exist
add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
	VERBATIM
)

# Copy the output .dll and debug .pdb to the configured output directory
add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}"
	VERBATIM
)