cmake_minimum_required (VERSION 3.8)
project (rt)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set (CMAKE_CXX_STANDARD 11)

add_executable(rt src/main.cpp src/Vectors/vectors.cpp src/Vectors/vectors.h src/misc/misc.cpp src/misc/misc.h src/Objects/Object.cpp src/Objects/Object.h src/Objects/Sphere.cpp src/Objects/Sphere.h src src/Objects/MeshTriangle.cpp src/Objects/MeshTriangle.h src/Objects/Light.h src/Scene.cpp src/Scene.h)

target_link_libraries(rt ${ALL_LIBS} )

