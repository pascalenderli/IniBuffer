cmake_minimum_required(VERSION 3.1...3.16)
project(IniBufferExampleApp VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#---------------------------------------------------------------------
# Build Configuration

set(build_python_package OFF)

#---------------------------------------------------------------------
# Build IniBuffer library

add_subdirectory(ini_buffer)

#---------------------------------------------------------------------
#Make Foleders for test data

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/results)
configure_file(../data/performance.ini data/performance.ini COPYONLY)
configure_file(../data/test.ini data/test.ini COPYONLY)

#---------------------------------------------------------------------
# Make example executable.

add_subdirectory(app)

#---------------------------------------------------------------------
# Make Python interace.

if(${build_python_package})
add_subdirectory(python_interface)
endif()
