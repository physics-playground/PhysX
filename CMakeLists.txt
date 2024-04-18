cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/cmakemodules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/targa)

set(CMAKE_TOOLCHAIN_FILE_DEFAULT $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

if(NOT EXISTS ${CMAKE_TOOLCHAIN_FILE_DEFAULT})
    set(CMAKE_TOOLCHAIN_FILE_DEFAULT ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

if(NOT EXISTS ${CMAKE_TOOLCHAIN_FILE_DEFAULT})
    set(CMAKE_TOOLCHAIN_FILE_DEFAULT ~/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE_DEFAULT}"
        CACHE STRING "Vcpkg toolchain file")

project(PhysXSDK)

IF(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT DEFINED $ENV{PM_DirectXSDK_VERSION})
    IF (EXISTS $ENV{DXSDK_DIR})
        SET(DIRECTX_INCLUDE_DIRS $ENV{DXSDK_DIR}/Include)
    ENDIF ()

    SET(DIRECTX_INCLUDE_DIRS ${DIRECTX_INCLUDE_DIRS} CACHE INTERNAL "DirectX SDK include path")

    find_path(directxsdk DIRECTX_SDK_PACKAGE CONFIG REQUIRED)
ENDIF ()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/physx/compiler/public/)
