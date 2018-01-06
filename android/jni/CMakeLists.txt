#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

set(F3D_ROOT "${CMAKE_SOURCE_DIR}/../..")
MESSAGE( STATUS "F3D_ROOT: " ${F3D_ROOT} )

# add cal3d static library
add_subdirectory(${F3D_ROOT}/src f3d)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DANDROID_NDK")

add_library(f3d_jni SHARED
            src/lights.cpp
            src/md2viewer.cpp
            src/stars.cpp)

target_include_directories(f3d_jni PRIVATE
                           ${F3D_ROOT}/include)

# add lib dependencies
target_link_libraries(f3d_jni
                      f3d
                      EGL
                      GLESv2
                      log)
