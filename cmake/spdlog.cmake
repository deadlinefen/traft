include(FetchContent)

if(NOT DEFINED SPDLOG_GIT_TAG)
  set(SPDLOG_GIT_TAG 1.15.0)
endif()

set(SPDLOG_GIT_URL  https://codeload.github.com/gabime/spdlog/tar.gz/refs/tags/v${SPDLOG_GIT_TAG})

FetchContent_Declare(
  spdlog
  URL               ${SPDLOG_GIT_URL}
  SOURCE_DIR        ${TRAFT_ROOT_PATH}/cmake-third-party/spdlog
)

FetchContent_GetProperties(spdlog)

if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)

  add_subdirectory(${TRAFT_ROOT_PATH}/cmake-third-party/spdlog)

  set_target_properties(
    spdlog  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${TRAFT_ROOT_PATH}/cmake-third-party/spdlog/include>"
  )
    
  add_library(traft_spdlog ALIAS spdlog)
    
  install(DIRECTORY ${spdlog_SOURCE_DIR}/include/spdlog
          COMPONENT DEVEL
          DESTINATION include/
          FILES_MATCHING PATTERN "*.h")
    
  set(TARGET_INCLUDE_PATHS    ${TARGET_INCLUDE_PATHS}
                              ${TRAFT_ROOT_PATH}/cmake-third-party/spdlog/include)
  set(TARGET_LINK_LIBS  ${TARGET_LINK_LIBS} traft_spdlog)

endif()
