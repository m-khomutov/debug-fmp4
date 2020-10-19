cmake_minimum_required(VERSION 3.10)
project(debug-mp4)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB DEBUG_MP4_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.hh ${PROJECT_SOURCE_DIR}/*.h)

add_executable(debug-mp4 ${DEBUG_MP4_SOURCES} atom.cpp atom.h freebox.cpp freebox.hh hvccbox.cpp hvccbox.hh)
