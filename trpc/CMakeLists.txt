file(GLOB TRPC_PATCH_SRC "coroutine/*.cc")

add_library(trpc-patch STATIC ${TRPC_PATCH_SRC})

message(STATUS "trpc-patch: ${TRPC_BASIC_3RD_PARTY}")

target_link_libraries(trpc-patch trpc protobuf 
protoc
gflags_nothreads
spdlog
fmt 
yaml-cpp 
jsoncpp 
pthread 
z 
dl  
curl)
