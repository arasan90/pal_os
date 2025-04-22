project(pal_osal_test C CXX)

include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

# Aggiungi il target per il test
add_executable(${PROJECT_NAME} pal_osal_thread_test.cpp)
# Collega Google Test al tuo test
target_link_libraries(${PROJECT_NAME} gtest gtest_main pal_os)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/private/include)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
