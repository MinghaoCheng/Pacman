cmake_minimum_required (VERSION 3.7)
project (Pacman_car)


set (PROJ_DIR "..")
set (SRC_DIR "${PROJ_DIR}/src")

set(EXECUTABLE_OUTPUT_PATH ${PROJ_DIR})


aux_source_directory(${SRC_DIR} MAIN_SRC)
aux_source_directory(${SRC_DIR}/test TEST_SRC)

add_executable(car ${MAIN_SRC} ${TEST_SRC})
