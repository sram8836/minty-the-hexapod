# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set Project Name
project(HexapodKinematics)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory to headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source Files
set(SOURCES
    src/main.cpp
    src/Brain.cpp
    src/Gaits.cpp
    src/InverseKinematics.cpp
    src/Leg.cpp
    src/periodicCallback.cpp
    src/ServoDriver.cpp
)

# Add Executable
add_executable(hexapod ${SOURCES})

# Link additional libraries
# target_link_libraries(hexapod <library1> ... <libraryn>)