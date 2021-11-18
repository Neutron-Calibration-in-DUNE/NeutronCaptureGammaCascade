
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(GammaCascade)

include_directories(GammaCascade PUBLIC ${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(cascade src/main.cc ${sources} ${headers})

add_custom_target(GammaCascade DEPENDS cascade)

set(CMAKE_BUILD_TYPE Debug)


