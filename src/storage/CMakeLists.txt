include_directories(${TRAFT_SRC_INCLUDE_DIR})

file(GLOB SOURCES "*.cc")

add_library(traft-storage STATIC ${SOURCES})
