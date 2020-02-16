cmake_minimum_required(VERSION 3.1...3.16)
project(IniBufferExampleApp VERSION 1.0 LANGUAGES CXX)

#---------------------------------------------------------------------
# Make example executable.

add_executable(${PROJECT_NAME}  cpp_example.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE IniBufferLib)
