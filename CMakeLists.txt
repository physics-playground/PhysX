cmake_minimum_required(VERSION 3.22)
project(PhysXSDK CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/cmakemodules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/externals/targa)

# this way others can override this value form command line
set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Path to toolchain")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/physx/compiler/public/)
