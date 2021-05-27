cmake_minimum_required (VERSION 3.4.1)

project(lz4)

include_directories(.)
file(GLOB SOURCES "*.c")

set (LIBRARY_TYPE STATIC)
if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    set (LIBRARY_TYPE SHARED)
elseif (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set (CMAKE_OSX_ARCHITECTURES "armv7;armv7s;arm64")
endif()

add_library(lz4 ${LIBRARY_TYPE}
${SOURCES}
)

target_include_directories(lz4 PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/..>
)

