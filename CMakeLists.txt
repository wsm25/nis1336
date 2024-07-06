cmake_minimum_required(VERSION 3.0)

project(schedule-cli
    VERSION 0.0.1
    DESCRIPTION "A smart schedule managing cli"
    HOMEPAGE_URL "https://github.com/wsm25/nis1336")

# dep: argon2
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/Argon2/libargon2.a
    WORKING_DIRECTORY  ${CMAKE_SOURCE_DIR}/Argon2/
    COMMAND make libargon2.a -j${CMAKE_BUILD_PARALLEL_LEVEL}
)

aux_source_directory(${CMAKE_SOURCE_DIR} DIR_SRCS)
add_executable(schedule-cli ${DIR_SRCS})
target_link_libraries(schedule-cli ${CMAKE_SOURCE_DIR}/Argon2/libargon2.a  "-static")
