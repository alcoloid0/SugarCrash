cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(SugarCrash VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

option(BUILD_JAR "Build JAR" ON)

if(NOT WIN32)
    message(FATAL_ERROR "Only Microsoft Windows supported")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Use a 64-bit compiler to build the project")
endif()

add_subdirectory(src)

if(BUILD_JAR)
    add_subdirectory(java)
endif()