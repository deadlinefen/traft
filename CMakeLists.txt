cmake_minimum_required(VERSION 3.14)
project(traft C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TRAFT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(TRAFT_SRC_PATH "${TRAFT_ROOT_PATH}/src")
set(CMAKE_MODULE_PATH "${TRAFT_ROOT_PATH}/cmake;${CMAKE_MODULE_PATH}")


include(FetchContent)
FetchContent_Declare(
    trpc-cpp
    GIT_REPOSITORY    git@git.woa.com:rayfordwu/trpc-cpp.git
    GIT_TAG           traft-v0.19.8
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/cmake-third-party/trpc-cpp
)
FetchContent_MakeAvailable(trpc-cpp)

include(xxhash)

set(PB_PROTOC ${PROTOBUF_PROTOC_EXECUTABLE})
set(TRPC_CPP_PLUGIN ${TRPC_TO_CPP_PLUGIN})

set(TRAFT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${TARGET_INCLUDE_PATHS}
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake-third-party/trpc-cpp)
include_directories(${TRAFT_INCLUDE_DIR})

find_program(LLD_EXECUTABLE lld)
if (LLD_EXECUTABLE)
    message(STATUS "LLD found: ${LLD_EXECUTABLE}")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=lld")
endif()

find_library(TCMALLOC_LIB tcmalloc)
if(TCMALLOC_LIB)
    message(STATUS "tcmalloc library found: ${TCMALLOC_LIB}")
endif()

add_subdirectory(traft)
