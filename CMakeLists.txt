cmake_minimum_required(VERSION 3.14)
project(BetterEditorCollab)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include the include directory for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the library target
add_library(BetterEditorCollab SHARED src/BetterEditorCollab.cpp)

# Install the library to the build directory
install(TARGETS BetterEditorCollab DESTINATION ${CMAKE_SOURCE_DIR}/build)
