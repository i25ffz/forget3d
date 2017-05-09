# CMake build file list for OpenAL

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

PROJECT(forget3d)

set(F3D_ROOT "${CMAKE_SOURCE_DIR}")
MESSAGE( STATUS "F3D_ROOT: " ${F3D_ROOT} )

# add cal3d static library
add_subdirectory(${F3D_ROOT}/src f3d)
add_subdirectory(${F3D_ROOT}/test)
