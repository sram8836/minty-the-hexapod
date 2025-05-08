# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set Project Name
project(HexapodKinematics)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source Files
set(SOURCES
    src/main.cpp
)

# Add Executable
add_executable(hexapod ${SOURCES})

# Link additional libraries
# target_link_libraries(hexapod <library1> ... <libraryn>)