set(TARGET_NAME "${PROJECT_NAME}-test")

set(SOURCES dummy.cpp main.cpp)

add_executable("${TARGET_NAME}" ${SOURCES})

target_link_libraries("${TARGET_NAME}" fmt::fmt Catch2::Catch2)

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
