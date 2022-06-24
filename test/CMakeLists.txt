set(TARGET_NAME "${PROJECT_NAME}-test")

set(SOURCES dummy.cpp main.cpp)

add_executable("${TARGET_NAME}" ${SOURCES})

# target

target_link_libraries("${TARGET_NAME}" fmt::fmt Catch2::Catch2)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)

# run

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
