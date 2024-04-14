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
IF(TARGET_BUILD_PLATFORM STREQUAL "" OR NOT DEFINED TARGET_BUILD_PLATFORM)
    if(WIN32)
        SET(TARGET_BUILD_PLATFORM "windows")
    else()
        SET(TARGET_BUILD_PLATFORM "linux")
    endif(WIN32)
endif()

IF(TARGET_BUILD_PLATFORM STREQUAL "mac" OR TARGET_BUILD_PLATFORM STREQUAL "ios")
    cmake_minimum_required(VERSION 3.12)
ELSE()
    cmake_minimum_required(VERSION 3.11)
ENDIF()

OPTION(PX_BUILDSNIPPETS "Generate the snippets" OFF)
OPTION(PX_BUILDPUBLICSAMPLES "Generate the samples" OFF)
OPTION(PX_CMAKE_SUPPRESS_REGENERATION "Disable zero_check projects" OFF)

file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR} PROJECT_ROOT_ABS_DIR_OPT1)
file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../.. PROJECT_ROOT_ABS_DIR_OPT2)
IF(EXISTS ${PROJECT_ROOT_ABS_DIR_OPT2}/README.md)
    file(TO_CMAKE_PATH ${PROJECT_ROOT_ABS_DIR_OPT2} PROJECT_ROOT_DIR)
ELSE()
    file(TO_CMAKE_PATH ${PROJECT_ROOT_ABS_DIR_OPT1} PROJECT_ROOT_DIR)
endif()

file(TO_CMAKE_PATH ${PROJECT_ROOT_DIR}/physx PHYSX_ROOT_DIR)

SET(PXSHARED_PATH ${PROJECT_ROOT_DIR}/pxshared)
SET(PM_PxShared_PATH ${PROJECT_ROOT_DIR}/pxshared)

SET($ENV{PM_CMakeModules_PATH} ${PROJECT_ROOT_DIR}/externals/cmakemodules)
SET(PM_CMakeModules_PATH ${PROJECT_ROOT_DIR}/externals/cmakemodules)

SET(PM_opengllinux_PATH ${PROJECT_ROOT_DIR}/externals/opengl-linux)
SET(PM_TARGA_PATH ${PROJECT_ROOT_DIR}/externals/targa)
SET(PM_CGLINUX_PATH ${PROJECT_ROOT_DIR}/externals/cg-linux)
SET(PM_GLEWLINUX_PATH ${PROJECT_ROOT_DIR}/externals/glew-linux)
SET(PM_PATHS ${PM_opengllinux_PATH};${PM_TARGA_PATH};${PM_CGLINUX_PATH};${PM_GLEWLINUX_PATH})
SET(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
SET(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)

IF(NOT DEFINED CMAKEMODULES_VERSION OR NOT EXISTS ${CMAKEMODULES_PATH})
    SET(CMAKEMODULES_PATH ${PM_CMakeModules_PATH} CACHE INTERNAL "Path to CMakeModules")
    SET(CMAKEMODULES_NAME "CMakeModules" CACHE INTERNAL "CMakeModules name")
    SET(CMAKEMODULES_VERSION "1.27" CACHE INTERNAL "CMakeModules version from generation batch")
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKEMODULES_PATH})

IF(PX_CMAKE_SUPPRESS_REGENERATION)
    SET(CMAKE_SUPPRESS_REGENERATION true)
ENDIF()

IF(NOT EXISTS ${PHYSX_ROOT_DIR})
    MESSAGE(FATAL_ERROR "PHYSX_ROOT_DIR variable was invalid: ${PHYSX_ROOT_DIR}")
ENDIF()

SET(PUBLIC_RELEASE 1)

# Enable folder properties
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Disable zero_check projects. The default for Switch and XboxOne is ON.
IF(PX_CMAKE_SUPPRESS_REGENERATION)
    SET(CMAKE_SUPPRESS_REGENERATION true)
ENDIF()

# Add PhysX SDK Source code to solution
ADD_SUBDIRECTORY(
        ${PHYSX_ROOT_DIR}/source/compiler/cmake
        ${CMAKE_CURRENT_BINARY_DIR}/sdk_source_bin)

MESSAGE("Added PhysX")

IF(PX_BUILDSNIPPETS)
    # Add Snippets projects into the solution
    ADD_SUBDIRECTORY(${PHYSX_ROOT_DIR}/snippets/compiler/cmake ${CMAKE_CURRENT_BINARY_DIR}/sdk_snippets_bin)

    MESSAGE("Added Snippets")
ENDIF()

IF(PX_BUILDPUBLICSAMPLES)
    IF(TARGET_BUILD_PLATFORM STREQUAL "windows" OR TARGET_BUILD_PLATFORM STREQUAL "linux")
        # Add samples projects into the solution
        ADD_SUBDIRECTORY(${PHYSX_ROOT_DIR}/samples/compiler/cmake ${CMAKE_CURRENT_BINARY_DIR}/sdk_samples_bin)

        MESSAGE("Added Samples")
    ENDIF()
ENDIF()

IF(TARGET_BUILD_PLATFORM STREQUAL "switch")
    FILE(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ALL_BUILD.vcxproj.user" INPUT "${CMAKE_MODULE_PATH}/switch/Microsoft.Cpp.${NX_TARGET_DEVKIT}.user.props" CONDITION 1)
    FILE(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/INSTALL.vcxproj.user" INPUT "${CMAKE_MODULE_PATH}/switch/Microsoft.Cpp.${NX_TARGET_DEVKIT}.user.props" CONDITION 1)
ENDIF()
