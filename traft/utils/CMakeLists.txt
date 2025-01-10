set(LOGGER_SRCS "logger.cc")

add_library(traft-logger STATIC ${LOGGER_SRCS})
link_libraries(traft-logger spdlog)
