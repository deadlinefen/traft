include(FetchContent)

if(NOT DEFINED CONCURRENTQUEUE_GIT_TAG)
  set(CONCURRENTQUEUE_GIT_TAG 1.0.4)
endif()

set(CONCURRENTQUEUE_GIT_URL  https://codeload.github.com/cameron314/concurrentqueue/tar.gz/refs/tags/v${CONCURRENTQUEUE_GIT_TAG})

FetchContent_Declare(
  concurrentqueue
  URL               ${CONCURRENTQUEUE_GIT_URL}
  SOURCE_DIR        ${TRAFT_ROOT_PATH}/cmake-third-party/concurrentqueue/include/concurrentqueue
)

FetchContent_GetProperties(concurrentqueue)

if(NOT concurrentqueue_POPULATED)
  FetchContent_Populate(concurrentqueue)

  add_subdirectory(${TRAFT_ROOT_PATH}/cmake-third-party/concurrentqueue/include/concurrentqueue)

  set_target_properties(
    concurrentqueue  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${TRAFT_ROOT_PATH}/cmake-third-party/concurrentqueue/include>"
  )
    
  add_library(traft_concurrentqueue ALIAS concurrentqueue)
    
  install(DIRECTORY ${concurrentqueue_SOURCE_DIR}
          COMPONENT DEVEL
          DESTINATION include/
          FILES_MATCHING PATTERN "*.h")
    
  set(TARGET_INCLUDE_PATHS    ${TARGET_INCLUDE_PATHS}
                              ${TRAFT_ROOT_PATH}/cmake-third-party/concurrentqueue/include)
  set(TARGET_LINK_LIBS  ${TARGET_LINK_LIBS} traft_concurrentqueue)

endif()
