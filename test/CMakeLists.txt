set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# triangle demo
add_executable (triangle triangle.cpp)

target_include_directories(triangle PRIVATE
                           ${F3D_ROOT}/include)
                           
target_link_libraries(triangle
                      f3d
                      X11
                      EGL
                      GLESv1_CM)
