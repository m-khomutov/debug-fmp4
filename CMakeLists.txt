cmake_minimum_required(VERSION 3.4)
project(debug-fmp4)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB DEBUG_FMP4_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.hh ${PROJECT_SOURCE_DIR}/*.h)

add_executable(debug-fmp4 ${DEBUG_FMP4_SOURCES} atom.cpp atom.h freebox.cpp freebox.hh hvccbox.cpp hvccbox.hh)
install(TARGETS debug-fmp4 DESTINATION /usr/local/bin)
