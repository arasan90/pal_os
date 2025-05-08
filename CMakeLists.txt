cmake_minimum_required(VERSION 3.25)

if(${TARGET_PLATFORM} STREQUAL "esp32")
    set(CMAKE_TOOLCHAIN_FILE $ENV{IDF_PATH}/tools/cmake/toolchain-esp32.cmake)
endif()

project(pal_os
    VERSION 0.1
    LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(EXPORT_COMPILE_COMMANDS ON)

set(ALLOWED_TARGETS "linux" "windows" "macos" "esp32")

if(NOT DEFINED TARGET_PLATFORM)
    set(TARGET_PLATFORM "linux" CACHE STRING "Target OS platform" FORCE)
    set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS linux esp32)
endif()

if(NOT TARGET_PLATFORM IN_LIST ALLOWED_TARGETS)
    message(FATAL_ERROR "Invalid TARGET_PLATFORM: '${TARGET_PLATFORM}'. Allowed values: linux esp32.")
endif()

if(${TARGET_PLATFORM} STREQUAL "linux")
    SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    SET(GCC_COVERAGE_LINK_FLAGS    "-coverage -lgcov")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
endif()

set(${PROJECT_NAME}_SRCS
    src/${TARGET_PLATFORM}/thread.c
    src/${TARGET_PLATFORM}/fs.c
    src/${TARGET_PLATFORM}/queue.c
    src/${TARGET_PLATFORM}/mutex.c
    src/${TARGET_PLATFORM}/signal.c
    src/${TARGET_PLATFORM}/socket.c
    src/${TARGET_PLATFORM}/system.c
    src/${TARGET_PLATFORM}/time.c
    src/${TARGET_PLATFORM}/timer.c
    )

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

if(NOT ${TARGET_PLATFORM} STREQUAL "esp32" AND PERFORM_UNIT_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()

if(${COMPILE_TEST_APP})
    if(${TARGET_PLATFORM} STREQUAL "esp32")
        # include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
        # idf_build_process(esp32)
        # target_link_libraries(${PROJECT_NAME} idf::log)
        # add_executable(${PROJECT_NAME}_dummy.elf private/dummy_app/main.c)
        # target_link_libraries(${PROJECT_NAME}_dummy.elf idf::newlib)
        # idf_build_executable(${PROJECT_NAME}_dummy.elf)
    else()
    add_subdirectory(app)
    endif()
endif()