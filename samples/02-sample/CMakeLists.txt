cmake_minimum_required(VERSION 2.8)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME testSampleCmd)

# set SOURCE_FILES
set(SOURCE_FILES
   testSampleCmd.cpp
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     OpenMayaAnim
     Foundation
)

# Build plugin
build_plugin()

