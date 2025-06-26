project(pal_osal_test C CXX)

include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

set(${PROJECT_NAME}_SRC
    pal_thread_test.cpp
    pal_queue_test.cpp
    pal_mutex_test.cpp
    pal_signal_test.cpp
    pal_system_test.cpp
    pal_time_test.cpp
    pal_timer_test.cpp
)

# Aggiungi il target per il test
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})

# Collega Google Test al tuo test
target_link_libraries(${PROJECT_NAME} gtest gtest_main pal_os)
target_include_directories(${PROJECT_NAME} PRIVATE ../src/${TARGET_PLATFORM})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
