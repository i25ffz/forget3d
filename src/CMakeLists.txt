set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DANDROID_NDK -DGL_GLEXT_PROTOTYPES -Wno-c++11-narrowing")
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

file(GLOB F3D_SRC_LIST *.cpp)

add_library(f3d STATIC
		${F3D_SRC_LIST})

target_include_directories(f3d PRIVATE
                           ${F3D_ROOT}/include)
