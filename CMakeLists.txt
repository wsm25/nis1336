cmake_minimum_required(VERSION 3.0)

project(schedule
    VERSION 0.0.1
    DESCRIPTION "A smart schedule managing cli"
    HOMEPAGE_URL "https://github.com/wsm25/nis1336")

# dep: hash
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/inc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRCS)
add_executable(${PROJECT_NAME} ${SRCS})
