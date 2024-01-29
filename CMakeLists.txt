#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(leap_sims)


set( CMAKE_BUILD_TYPE "Debug")
SET( CMAKE_CXX_FLAGS_DEBUG  "-g")
#----------------------------------------------------------------------------
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

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
#include_directories(${PROJECT_SOURCE_DIR}/include
#                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(leap_sims leap_sims.cc ${sources} ${headers})
# set_source_files_properties( ${PROJECT_SOURCE_DIR}/src/*.cc PROPERTIES COMPILE_FLAGS "-g")
target_link_libraries(leap_sims ${Geant4_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Pol01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(LEAP_SIMS_SCRIPTS
    vis.mac
    init_vis.mac
    gui.mac
    test_changes.mac
  )

foreach(_script ${LEAP_SIMS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS leap_sims DESTINATION bin)
