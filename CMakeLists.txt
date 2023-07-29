# ┏┓┳┳┓  ┓   ┓ •   
# ┃ ┃┃┃┏┓┃┏┏┓┃ ┓┏╋┏
# ┗┛┛ ┗┗┻┛┗┗ ┗┛┗┛┗┛
# Configure all compilation Settings for the simulation
# ------------------------------------------------------

# Cmake Requirement
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Project Name
project(APXS)

# Create an output directory in the build folder
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# Copy the macros in the target directory
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})

# Load Geant4 and include its headers
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Find the source code and headers for the simulation
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create the excecutable by compiling our uncompiled code
add_executable(apxs apxs.cc ${sources} ${headers})

# Link the already compiled code into our excecutable
target_link_libraries(apxs ${Geant4_LIBRARIES})
