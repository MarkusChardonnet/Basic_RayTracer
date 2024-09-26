cmake_minimum_required(VERSION 3.1)
project(MyRayTracer)

set(SRCS main.cpp Image.cpp Scene.cpp utils.cpp View.cpp Vec3.cpp)
set(HEADERS Image.h Scene.h utils.h View.h)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

target_link_libraries(${PROJECT_NAME})

message (${CMAKE_C_COMPILER})
message (${CMAKE_CXX_COMPILER})
message (${CMAKE_CXX_COMPILER_AR})
message (${CMAKE_CXX_COMPILER_RANLIB})