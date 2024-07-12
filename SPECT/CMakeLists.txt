# (1)----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(SPECT)

# (2)----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# (3)----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#

# Geant4_USE_FILE is the path to the module located by the previous find_package
include(${Geant4_USE_FILE})

# add the header directory to the complier's header search path
include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${Geant4_INCLUDE_DIRS})

# (4)----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# (5)----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(run run.cc ${sources} ${headers})
target_link_libraries(run ${Geant4_LIBRARIES})


# (6)----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SCRIPTS
  init_vis.mac
  vis.mac
  )

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
