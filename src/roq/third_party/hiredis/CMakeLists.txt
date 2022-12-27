set(TARGET_NAME "${PROJECT_NAME}-hiredis")

set(SOURCES context.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" PRIVATE roq-api::roq-api fmt::fmt ${HIREDIS_LIBRARIES})

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)
