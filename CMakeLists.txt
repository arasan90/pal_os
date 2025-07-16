cmake_minimum_required(VERSION 3.10)

if(PAL_OS_DEV)
if(${TARGET_PLATFORM} STREQUAL "linux")
    add_compile_definitions(PAL_OS_LINUX)
elseif(${TARGET_PLATFORM} STREQUAL "freeRTOS")
    add_compile_definitions(PAL_OS_FREERTOS)
endif()
    project(pal_os LANGUAGES C VERSION 1.0.0)

    include(${CMAKE_CURRENT_LIST_DIR}/pal_os.cmake)
    include(FetchContent)
        FetchContent_Declare(
            gtest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.16.0
        )

    set(pal_os_sources)
    set(pal_os_public_include_dirs)
    set(pal_os_private_include_dirs)
    set(pal_os_private_linked_libs)
    pal_os_get_sources(pal_os_sources)
    pal_os_get_public_headers(pal_os_public_include_dirs)
    pal_os_get_private_headers(pal_os_private_include_dirs)
    pal_os_get_private_linked_libs(pal_os_private_linked_libs)

    add_library(${PROJECT_NAME} STATIC ${pal_os_sources})
    target_include_directories(${PROJECT_NAME} PUBLIC ${pal_os_public_include_dirs})
    target_include_directories(${PROJECT_NAME} PRIVATE ${pal_os_private_include_dirs})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${pal_os_private_linked_libs})


    SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    SET(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

    FetchContent_MakeAvailable(gtest)
    enable_testing()
    include(CTest)

    add_subdirectory(test)

    pal_os_create_mock_library()
else()
    message(FATAL_ERROR "This cmake file is only for development purposes. Please use the main CMakeLists.txt file for building the library.")

endif()