find_package(Protobuf REQUIRED)
if (NOT Protobuf_FOUND)
    message(FATAL_ERROR "Protobuf not found")
endif()

# add_subdirectory(storage)
# add_subdirectory(raft)
add_subdirectory(utils)
add_subdirectory(testing)

set(SUB_LIBS ${SUB_LIBS} 
  # storage 
  # raft
)

add_library(OBJ_LIB OBJECT "raft.cc")

set_property(TARGET ${OBJ_LIB} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(traft-shared SHARED $<TARGET_OBJECTS:OBJ_LIB>)
add_library(traft-static STATIC $<TARGET_OBJECTS:OBJ_LIB>)

target_link_libraries(traft-shared ${SUB_LIBS})
target_link_libraries(traft-static ${SUB_LIBS})

install(TARGETS traft-shared
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIBSUFFIX}
        ARCHIVE DESTINATION lib${LIBSUFFIX}
)

install(TARGETS traft-static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIBSUFFIX}
        ARCHIVE DESTINATION lib${LIBSUFFIX}
)
