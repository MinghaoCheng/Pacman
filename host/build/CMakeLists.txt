# project
project (Pacman_car)
cmake_minimum_required (VERSION 3.7)

# build type& parameters
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall")

# dir specification
set (PROJ_DIR "..")
set (SRC_DIR "${PROJ_DIR}/src")

# add folders of source files
aux_source_directory(${SRC_DIR} MAIN_SRC)
aux_source_directory(${SRC_DIR}/BSP BSP_SRC)
aux_source_directory(${SRC_DIR}/Drivers DRIVER_SRC)
aux_source_directory(${SRC_DIR}/util UTIL_SRC)

# find packages
find_package (Threads)
#find_package(ALSA REQUIRED)
#add_definitions(-D__LINUX_ALSA__)

# include directories
#include_directories(${ALSA_INCLUDE_DIRS})

# set exe output path
# set(EXECUTABLE_OUTPUT_PATH ${PROJ_DIR})

# add exe
add_executable(host ${MAIN_SRC} ${BSP_SRC} ${DRIVER_SRC} ${UTIL_SRC})
target_link_libraries (host ${CMAKE_THREAD_LIBS_INIT} rt)
#target_link_libraries (host ${CMAKE_THREAD_LIBS_INIT} ${ALSA_LIBRARIES} rt)
