set(TARGET_NAME "${PROJECT_NAME}-event")

set(SOURCES base.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" fmt::fmt ${LIBEVENT_LIBRARIES})

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)
