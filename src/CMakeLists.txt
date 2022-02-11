set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCE_DIR "${ROOT_DIR}/src")
set(SOURCE_FILES
	"${SOURCE_DIR}/main.cpp"
	"${SOURCE_DIR}/PCH.h"
	"${SOURCE_DIR}/Hooks.h"
	"${SOURCE_DIR}/Hooks.cpp"
	"${SOURCE_DIR}/RuntimeEvents.h"
	"${SOURCE_DIR}/RuntimeEvents.cpp"
	"${SOURCE_DIR}/Papyrus.h"
	"${SOURCE_DIR}/Papyrus.cpp"
	"${SOURCE_DIR}/ArousalManager.h"
	"${SOURCE_DIR}/ArousalManager.cpp"
	"${SOURCE_DIR}/LibidoManager.h"
	"${SOURCE_DIR}/LibidoManager.cpp"
	"${SOURCE_DIR}/SceneManager.h"
	"${SOURCE_DIR}/SceneManager.cpp"
	"${SOURCE_DIR}/SavedDataManager.h"
	"${SOURCE_DIR}/SavedDataManager.cpp"
	"${SOURCE_DIR}/Utilities.h"
	"${SOURCE_DIR}/Utilities.cpp"
	"${SOURCE_DIR}/Settings.h"
	"${SOURCE_DIR}/Debug.h"
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
	${ROOT_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/Version.h
	@ONLY
)

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
		cxx_std_20
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

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/src"
		"${SOURCE_DIR}"
)

if (DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${ROOT_DIR}${CommonLibPath})
	add_subdirectory(${ROOT_DIR}${CommonLibPath} ${CommonLibName} EXCLUDE_FROM_ALL)
else ()
	message(
		FATAL_ERROR
		"Variable ${CommonLibPath} Path "${ROOT_DIR}/external/CommonLibSSE" is not set."
	)
endif()

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		CommonLibSSE::CommonLibSSE
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/PCH.h"
)

install(
	FILES
		"$<TARGET_FILE:${PROJECT_NAME}>"
	DESTINATION "SKSE/Plugins"
	COMPONENT "main"
)

install(
	FILES
		"$<TARGET_PDB_FILE:${PROJECT_NAME}>"
	DESTINATION "/"
	COMPONENT "pdbs"
)

add_custom_command(
	TARGET "${PROJECT_NAME}"
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "D:/Skyrim/Mods/OSLAroused/SKSE/Plugins/"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "D:/Skyrim/Mods/OSLAroused/SKSE/Plugins/"
	VERBATIM
)
