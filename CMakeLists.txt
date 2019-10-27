cmake_minimum_required(VERSION 3.13.0)

project(Platformer)

set(CMAKE_CXX_STANDARD 17)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/x64)
set(BINARIES_DIR ${PROJECT_SOURCE_DIR}/bin)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build)

include(cotire)

add_subdirectory(Engine)
add_subdirectory(Platformer)
