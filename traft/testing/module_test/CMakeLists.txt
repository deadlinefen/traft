file(GLOB TEST_SOURCES "*_test.cc")

foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} trpc ${TCMALLOC_LIB})
endforeach()
