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

# Copy the macros directory
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})

# Create the option to enable 3D model support using GDML
option(GDML_DETECTOR "Compile using the 3D model of the SDD through GDML" ON)
if(GDML_DETECTOR)

# Copy the directory with the detector geometry and data to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/geometry DESTINATION ${PROJECT_BINARY_DIR})

# Create the appropriate definitions to the compiler
add_compile_definitions(DETECTOR_GDML)
# add_compile_definitions(DETECTOR_GDML_FILENAME="${PROJECT_BINARY_DIR}/geometry/sdd.gdml")
endif()


option(X_RAY "X-ray tube vs disks. If ON, no disks" OFF)
if(X_RAY)
add_compile_definitions(X_RAY)
endif()

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


# Add the option to compile using G4MPI for parallel execution (on clusters probably)
option(ENABLE_MPI "Compile using G4MPI to enable multiprocessing!" OFF)
if(ENABLE_MPI)

# Define the MPI_ENABLE Flag on the compiler so that we can pick 
# the appropriate code on the main function
add_compile_definitions(MPI_ENABLE)

# Try and steal the G4MPI directory form an environment variable first
set(G4mpi_DIR $ENV{G4MPI_DIR})

# Find the G4mpi package
find_package(G4mpi REQUIRED)

# Flag for using native mpi
add_definitions(-DTOOLS_USE_NATIVE_MPI) 

# Include mpi directories
include_directories(${G4mpi_INCLUDE_DIR})

# link G4MPI to the executable we are making
target_link_libraries(apxs ${G4mpi_LIBRARIES})
endif()