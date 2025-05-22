cmake_minimum_required(VERSION 3.25)

project(pal_os
    VERSION 0.1
    LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ALLOWED_TARGETS "linux" "windows" "macos" "freeRTOS")

if(NOT DEFINED TARGET_PLATFORM)
    set(TARGET_PLATFORM "linux" CACHE STRING "Target OS platform" FORCE)
    set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS linux freeRTOS)
endif()

if(NOT TARGET_PLATFORM IN_LIST ALLOWED_TARGETS)
    message(FATAL_ERROR "Invalid TARGET_PLATFORM: '${TARGET_PLATFORM}'. Allowed values: linux freeRTOS.")
endif()
add_library(${PROJECT_NAME})
if(${TARGET_PLATFORM} STREQUAL "linux")
    SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    SET(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    add_definitions(-DPAL_OS_LINUX)
    set(${PROJECT_NAME}_SRCS
        src/${TARGET_PLATFORM}/thread.c
        src/${TARGET_PLATFORM}/queue.c
        src/${TARGET_PLATFORM}/mutex.c
        src/${TARGET_PLATFORM}/signal.c
        src/${TARGET_PLATFORM}/system.c
        src/${TARGET_PLATFORM}/time.c
        src/${TARGET_PLATFORM}/timer.c
    )
    set(${PROJECT_NAME}_LIBS)
elseif(${TARGET_PLATFORM} STREQUAL "freeRTOS")
    add_definitions(-DPAL_OS_FREERTOS)
    set(${PROJECT_NAME}_SRCS
        src/freeRTOS/thread.c
        src/freeRTOS/queue.c
        src/freeRTOS/mutex.c
        src/freeRTOS/signal.c
        src/freeRTOS/system.c
        src/freeRTOS/time.c
        src/freeRTOS/timer.c
    )
    target_link_libraries(${PROJECT_NAME} pal_os_freertos)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

if(NOT ${TARGET_PLATFORM} STREQUAL "freeRTOS" AND PERFORM_UNIT_TESTS)
    if(PERFORM_UNIT_TESTS)
        include(CTest)
        add_subdirectory(test)
    endif()
    if(COMPILE_MOCK)
        add_subdirectory(libs/fff)
        add_subdirectory(mock)
    endif()
endif()