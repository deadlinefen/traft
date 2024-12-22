file(GLOB PROTO_SRCS "*.proto")

add_library(raft-proto OBJECT "${PROTO_SRCS}")

target_link_libraries(raft-proto PUBLIC protobuf::libprotobuf)

target_include_directories(raft-proto PUBLIC "$<BUILD_INTERFACE:${SUB_PROTO_INCLUDE_DIR}>")

file(MAKE_DIRECTORY "${SUB_PROTO_OUTPUT_DIR}")

protobuf_generate(
    TARGET raft-proto
    LANGUAGE cpp
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    PROTOC_OUT_DIR "${SUB_PROTO_OUTPUT_DIR}"
)
