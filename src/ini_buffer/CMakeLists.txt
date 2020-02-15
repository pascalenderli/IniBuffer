project(IniBufferLib VERSION 1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
ini_buffer.h
ini_buffer.cpp
str_manip.h)

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

