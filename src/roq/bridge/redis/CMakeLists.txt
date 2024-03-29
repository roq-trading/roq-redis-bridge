if(BUILD_TESTING)
  add_subdirectory(test)
endif()

set(TARGET_NAME ${PROJECT_NAME})

add_subdirectory(flags)

set(SOURCES application.cpp config.cpp controller.cpp settings.cpp main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE ${TARGET_NAME}-flags
          ${PROJECT_NAME}-hiredis
          roq-client::roq-client
          roq-client::roq-client-flags
          roq-io::roq-io
          roq-market::roq-market
          roq-cache::roq-cache
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          fmt::fmt
          unordered_dense::unordered_dense
          ${HIREDIS_LIBRARIES}
          ${RT_LIBRARIES})

if(ROQ_BUILD_TYPE STREQUAL "Release")
  set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

target_compile_definitions(
  ${TARGET_NAME}
  PRIVATE ROQ_PACKAGE_NAME="${TARGET_NAME}" ROQ_BUILD_VERSION="${GIT_REPO_VERSION}"
          ROQ_GIT_DESCRIBE_HASH="${GIT_DESCRIBE_HASH}" ROQ_BUILD_NUMBER="${ROQ_BUILD_NUMBER}"
          ROQ_BUILD_TYPE="${ROQ_BUILD_TYPE}")

install(TARGETS ${TARGET_NAME})
