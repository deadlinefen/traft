cmake_minimum_required(VERSION 3.14)
project(traft C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TRAFT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(TRAFT_SRC_PATH "${TRAFT_ROOT_PATH}/src")
set(CMAKE_MODULE_PATH "${TRAFT_ROOT_PATH}/cmake;${CMAKE_MODULE_PATH}")

include(fmt)
include(spdlog)
include(concurrentqueue)
include(xxhash)

set(TRAFT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} 
                      ${TARGET_INCLUDE_PATHS})
include_directories(${TRAFT_INCLUDE_DIR})

add_subdirectory(traft)
