cmake_minimum_required (VERSION 3.8)
find_package(Git)

# project(StarlightCommon VERSION 1.0.0 LANGUAGES CXX)
project("ShaderManager")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MACOSX_RPATH 1)
cmake_policy(SET CMP0076 NEW)

set(SMM_SOURCES "include/ShaderManager.h" "include/Compiler.h")
set(SMM_HEADERS "src/ShaderManager.cpp" "src/Compiler.cpp")

if (GIT_FOUND)
	if(GIT_SUBMODULE AND PULL_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()

	if(PREP_EXTERNAL_PACKAGES)
		message("Preparing requirenments for external dependencies")
		message("Preparing dependencies for spirvtools")
		find_library(EFFCEE_LIB NAME effcee HINTS ${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external)
		if (NOT EFFCEE_LIB AND NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external/effcee")
			message("effcee is required for spirvtools and was not found, fetching...")
			execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/google/effcee.git
				WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external"
				RESULT_VARIABLE GIT_EFFCEE_RESULT
			)
			if (NOT GIT_EFFCEE_RESULT EQUAL "0")
				message(FATAL_ERROR "effcee is needed for building spirvtools, fetch into required dir")
			endif()
		endif()

		find_library(RE2_LIB NAME re2 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external)
		if (NOT RE2_LIB AND NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external/re2")
			message("re2 is required for spirvtools and was not found, fetching...")
			execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/google/re2.git
				WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/spirvtools/external"
				RESULT_VARIABLE GIT_RE2_RESULT
			)
			if (NOT GIT_RE2_RESULT EQUAL "0")
				message(FATAL_ERROR "re2 is needed for building spirvtools, fetch into required dir")
			endif()
		endif()
	endif()
	message("Prep Complete")
endif()

# target_include_directories(${PROJECT_NAME} PUBLIC 
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# )
# add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME} STATIC ${SMM_SOURCES})

set(SPIRV_HEADERS_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)
add_subdirectory("external/spirvheaders")

set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_TESTS ON CACHE BOOL "" FORCE)
add_subdirectory("external/spirvtools")

set(ENABLE_CTEST OFF CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/glslang")

set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_ENABLE_SHARED_CRT ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_COPYRIGHT_CHECK ON CACHE BOOL "" FORCE)
add_subdirectory("external/shaderc")

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${COMMON_INCLUDE_DIR}
		"external/shaderc/libshaderc/include"
		"include"
)
#[[
target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        # "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        "${PROJECT_SOURCE_DIR}/external/spirvtools"
		"external/shaderc/libshaderc/include"
		

    PUBLIC
        include 
        # where top-level project will look for the library's public headers
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        #$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)]]

target_sources( ${PROJECT_NAME}
    PUBLIC 
        ${SMM_HEADERS}
    PRIVATE 
        ${SMM_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")