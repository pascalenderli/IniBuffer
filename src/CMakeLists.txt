cmake_minimum_required(VERSION 3.1...3.16)
project(IniBufferExampleApp VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

ADD_SUBDIRECTORY(ini_buffer)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/results)
configure_file(../data/performance.ini data/performance.ini COPYONLY)
configure_file(../data/test.ini data/test.ini COPYONLY)

add_executable(${PROJECT_NAME}  main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE IniBufferLib)

