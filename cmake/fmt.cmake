#
#
# Tencent is pleased to support the open source community by making tRPC available.
#
# Copyright (C) 2023 THL A29 Limited, a Tencent company.
# All rights reserved.
#
# If you have downloaded a copy of the tRPC source code from Tencent,
# please note that tRPC source code is licensed under the  Apache 2.0 License,
# A copy of the Apache 2.0 License is included in this file.
#
#

include(FetchContent)

if(NOT DEFINED FMT_GIT_TAG)
  set(FMT_GIT_TAG 11.0.2)
endif()

set(FMT_GIT_URL  https://github.com/fmtlib/fmt/archive/${FMT_GIT_TAG}.tar.gz)

FetchContent_Declare(
  fmt
  URL               ${FMT_GIT_URL}
  SOURCE_DIR        ${TRAFT_ROOT_PATH}/cmake-third-party/fmt
)

FetchContent_GetProperties(fmt)

if(NOT fmt_POPULATED)
  FetchContent_Populate(fmt)

  add_subdirectory(${TRAFT_ROOT_PATH}/cmake-third-party/fmt)

  set_target_properties(
    fmt  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${TRAFT_ROOT_PATH}/cmake-third-party/fmt/include>"
  )
    
  add_library(traft_fmt ALIAS fmt)
    
  install(DIRECTORY ${fmt_SOURCE_DIR}/include/fmt
          COMPONENT DEVEL
          DESTINATION include/
          FILES_MATCHING PATTERN "*.h")
    
  set(TARGET_INCLUDE_PATHS    ${TARGET_INCLUDE_PATHS}
                              ${TRAFT_ROOT_PATH}/cmake-third-party/fmt/include)
  set(TARGET_LINK_LIBS  ${TARGET_LINK_LIBS} traft_fmt)

endif()
