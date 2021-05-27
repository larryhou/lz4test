cmake_minimum_required (VERSION 3.4.1)

project(lz4test)

set (CMAKE_CXX_STANDARD 11)

include_directories(lz4test)

if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set (CMAKE_OSX_ARCHITECTURES "armv7;armv7s;arm64")
endif()

add_subdirectory(external/lz4)

add_library(lz4test lz4test/lz4test.cpp )
target_link_libraries(lz4test lz4)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    add_executable(lz4test_ lz4test/lz4test.cpp lz4test/main.cpp)
    set (CMAKE_BUILD_TYPE "Release")
    target_link_libraries(lz4test_ lz4)
    add_custom_command(TARGET lz4test_ POST_BUILD
        COMMAND mv -fv ${CMAKE_BINARY_DIR}/lz4test_ ${CMAKE_BINARY_DIR}/lz4test
    )
endif()

file(GLOB PUBLIC_HEADERS "lz4test/*.h*" )

file(COPY ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_BINARY_DIR}/include
)

