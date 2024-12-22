include(FetchContent)

if(NOT DEFINED XXHASH_GIT_TAG)
  set(XXHASH_GIT_TAG 0.8.2)
endif()

set(XXHASH_GIT_URL  https://codeload.github.com/Cyan4973/xxHash/tar.gz/refs/tags/v${XXHASH_GIT_TAG})

FetchContent_Declare(
  xxHash
  URL               ${XXHASH_GIT_URL}
  SOURCE_DIR        ${TRAFT_ROOT_PATH}/cmake-third-party/xxHash/include/xxHash
)

FetchContent_GetProperties(xxHash)

if(NOT xxHash_POPULATED)
  FetchContent_Populate(xxHash)
    
  install(DIRECTORY ${xxHash_SOURCE_DIR}/xxHash/include
          COMPONENT DEVEL
          DESTINATION include/
          FILES_MATCHING PATTERN "*.h")
    
  set(TARGET_INCLUDE_PATHS    ${TARGET_INCLUDE_PATHS}
                              ${TRAFT_ROOT_PATH}/cmake-third-party/xxHash/include)

endif()
