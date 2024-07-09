cmake_minimum_required(VERSION 3.0)

project(schedule
    VERSION 0.0.1
    DESCRIPTION "A smart schedule managing cli"
    HOMEPAGE_URL "https://github.com/wsm25/nis1336")

# dep: hash
include_directories(${CMAKE_SOURCE_DIR}/Hash/src)
add_executable(${PROJECT_NAME} 
    cli.cc storage.cc user.cc task.cc)

# tests
enable_testing()

add_executable(storage_test storage_test.cc storage.cc user.cc)
add_test(NAME storage_test COMMAND $<TARGET_FILE:storage_test>)