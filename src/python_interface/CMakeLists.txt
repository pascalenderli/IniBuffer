cmake_minimum_required(VERSION 3.1...3.16)
project(IniBuffer VERSION 1.0 LANGUAGES CXX)

#---------------------------------------------------------------------
# Make Python interface.

pybind11_add_module(${PROJECT_NAME} python_interface.cpp ini_buffer_py.h)
target_link_libraries(${PROJECT_NAME} PRIVATE IniBufferLib)

# Copy python application example to binary folder
configure_file(../app/python_example.py ../app/python_example.py COPYONLY)
configure_file(../app/python_example.py python_example.py COPYONLY)
configure_file(__init__.py __init__.py COPYONLY)

#---------------------------------------------------------------------
# Notes for making the python interface:
# required stuff before building:
# sudo apt install python3-dev
# sudo apt install python3-distutils
#
# Required stuff after building:
# Copy the shared labrary or make a symlink from from "/usr/lib/python3/dist-packages" to the shared .so lib.
# /usr/lib/python3/dist-packages
