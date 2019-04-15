# project
project (Pacman_car)
cmake_minimum_required (VERSION 3.7)

# build type& parameters
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall")

# dir specification
set (PROJ_DIR "..")
set (SRC_DIR "${PROJ_DIR}/src")

# add folders of source files
aux_source_directory(${SRC_DIR} MAIN_SRC)

# set exe output path
#set(EXECUTABLE_OUTPUT_PATH ${PROJ_DIR})

# add exe
add_executable(car ${MAIN_SRC})
target_link_libraries (car wiringPi pthread)