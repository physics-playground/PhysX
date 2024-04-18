# #
# # Redistribution and use in source and binary forms, with or without
# # modification, are permitted provided that the following conditions
# # are met:
# #  * Redistributions of source code must retain the above copyright
# #    notice, this list of conditions and the following disclaimer.
# #  * Redistributions in binary form must reproduce the above copyright
# #    notice, this list of conditions and the following disclaimer in the
# #    documentation and/or other materials provided with the distribution.
# #  * Neither the name of NVIDIA CORPORATION nor the names of its
# #    contributors may be used to endorse or promote products derived
# #    from this software without specific prior written permission.
# #
# # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
# # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# #
# # Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.

cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/cmakemodules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/targa)

set(CMAKE_TOOLCHAIN_FILE_DEFAULT $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE_DEFAULT})
	set(CMAKE_TOOLCHAIN_FILE_DEFAULT ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif ()

if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE_DEFAULT})
	set(CMAKE_TOOLCHAIN_FILE_DEFAULT ~/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif ()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE_DEFAULT}"
	CACHE STRING "Vcpkg toolchain file")

project(nvidia-gameworks-physx)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT DEFINED $ENV{PM_DirectXSDK_VERSION})
	include(FindPackageHandleStandardArgs)
	
	if (EXISTS $ENV{DXSDK_DIR})
		set(DXSDK_INCLUDE_DIRS $ENV{DXSDK_DIR}/Include)
	endif ()

	# find_package_handle_standard_args(directxsdk DIRECTX_SDK_PACKAGE CONFIG REQUIRED)
	find_package(directxsdk CONFIG REQUIRED)

	mark_as_advanced(DIRECTXSDK_H_PATH DIRECTXSDK_LIB_PATH DIRECTXSDK_ARCH)

	# set up output vars
	if (DIRECTXSDK_FOUND)
		set(DIRECTX_INCLUDE_DIRS ${DIRECTXSDK_H_PATH} CACHE INTERNAL "DirectX SDK include paths")
		set(DIRECTX_LIBRARY_DIRS ${DIRECTXSDK_LIB_PATH} CACHE INTERNAL "DirectX SDK library paths")
	else ()
		set(DIRECTX_INCLUDE_DIRS ${DXSDK_INCLUDE_DIRS} CACHE INTERNAL "DirectX SDK include paths")
		set(DIRECTX_LIBRARY_DIRS CACHE INTERNAL "DirectX SDK library paths")
	endif ()
endif ()

if (TARGET_BUILD_PLATFORM STREQUAL "" OR NOT DEFINED TARGET_BUILD_PLATFORM)
	if (WIN32)
		set(TARGET_BUILD_PLATFORM "windows")
	else ()
		set(TARGET_BUILD_PLATFORM "linux")
	endif (WIN32)
endif ()

if (TARGET_BUILD_PLATFORM STREQUAL "mac" OR TARGET_BUILD_PLATFORM STREQUAL "ios")
	cmake_minimum_required(VERSION 3.12)
else ()
	cmake_minimum_required(VERSION 3.11)
endif ()

option(PX_BUILDSNIPPETS "Generate the snippets" ON)
option(PX_BUILDPUBLICSAMPLES "Generate the samples" ON)
option(PX_CMAKE_SUPPRESS_REGENERATION "Disable zero_check projects" OFF)

file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR} PROJECT_ROOT_ABS_DIR_OPT1)
file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../.. PROJECT_ROOT_ABS_DIR_OPT2)

if (EXISTS ${PROJECT_ROOT_ABS_DIR_OPT2}/README.md)
	file(TO_CMAKE_PATH ${PROJECT_ROOT_ABS_DIR_OPT2} PROJECT_ROOT_DIR)
else ()
	file(TO_CMAKE_PATH ${PROJECT_ROOT_ABS_DIR_OPT1} PROJECT_ROOT_DIR)
endif ()

file(TO_CMAKE_PATH ${PROJECT_ROOT_DIR}/physx PHYSX_ROOT_DIR)

set(PXSHARED_PATH ${PROJECT_ROOT_DIR}/pxshared)
set(PM_PxShared_PATH ${PROJECT_ROOT_DIR}/pxshared)

set(CMAKE_PREFIX_PATH "${PROJECT_ROOT_DIR}/externals/cmakemodules;${PROJECT_ROOT_DIR}/externals;${PROJECT_ROOT_DIR}/externals/targa" FORCE)
set(CMAKE_MODULE_PATH_PHYSX ${PROJECT_ROOT_DIR}/externals/cmakemodules)
set($ENV{PM_CMakeModules_PATH} ${CMAKE_MODULE_PATH_PHYSX})
set(PM_CMakeModules_PATH ${CMAKE_MODULE_PATH_PHYSX})

set(PM_opengllinux_PATH ${PROJECT_ROOT_DIR}/externals/opengl-linux)
set(PM_TARGA_PATH ${PROJECT_ROOT_DIR}/externals/targa)
set(PM_CGLINUX_PATH ${PROJECT_ROOT_DIR}/externals/cg-linux)
set(PM_GLEWLINUX_PATH ${PROJECT_ROOT_DIR}/externals/glew-linux)
set(PM_PATHS ${PM_opengllinux_PATH};${PM_TARGA_PATH};${PM_CGLINUX_PATH};${PM_GLEWLINUX_PATH})
set(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)

if (NOT DEFINED CMAKEMODULES_VERSION OR NOT EXISTS ${CMAKEMODULES_PATH})
	set(CMAKEMODULES_PATH ${PM_CMakeModules_PATH} CACHE INTERNAL "Path to CMakeModules")
	set(CMAKEMODULES_NAME "CMakeModules" CACHE INTERNAL "CMakeModules name")
	set(CMAKEMODULES_VERSION "1.27" CACHE INTERNAL "CMakeModules version from generation batch")
endif ()

set(CMAKE_MODULE_PATH ${CMAKEMODULES_PATH})

if (PX_CMAKE_SUPPRESS_REGENERATION)
	set(CMAKE_SUPPRESS_REGENERATION true)
endif ()

if (NOT EXISTS ${PHYSX_ROOT_DIR})
	message(FATAL_ERROR "PHYSX_ROOT_DIR variable was invalid: ${PHYSX_ROOT_DIR}")
endif ()

set(PUBLIC_RELEASE 0)

# Enable folder properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Disable zero_check projects. The default for Switch and XboxOne is ON.
if (PX_CMAKE_SUPPRESS_REGENERATION)
	set(CMAKE_SUPPRESS_REGENERATION true)
endif ()

# Add PhysX SDK Source code to solution
add_subdirectory(
		${PHYSX_ROOT_DIR}/source/compiler/cmake
		${CMAKE_CURRENT_BINARY_DIR}/sdk_source_bin)

message("Added PhysX")

if (PX_BUILDSNIPPETS)
	# Add Snippets projects into the solution
	add_subdirectory(${PHYSX_ROOT_DIR}/snippets/compiler/cmake ${CMAKE_CURRENT_BINARY_DIR}/sdk_snippets_bin)

	message("Added Snippets")
endif ()

if (PX_BUILDPUBLICSAMPLES)
	if (TARGET_BUILD_PLATFORM STREQUAL "windows" OR TARGET_BUILD_PLATFORM STREQUAL "linux")
		# Add samples projects into the solution
		add_subdirectory(${PHYSX_ROOT_DIR}/samples/compiler/cmake ${CMAKE_CURRENT_BINARY_DIR}/sdk_samples_bin)

		message("Added Samples")
	endif ()
endif ()

if (TARGET_BUILD_PLATFORM STREQUAL "switch")
	file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ALL_BUILD.vcxproj.user" INPUT "${CMAKE_MODULE_PATH}/switch/Microsoft.Cpp.${NX_TARGET_DEVKIT}.user.props" CONDITION 1)
	file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/INSTALL.vcxproj.user" INPUT "${CMAKE_MODULE_PATH}/switch/Microsoft.Cpp.${NX_TARGET_DEVKIT}.user.props" CONDITION 1)
endif ()
