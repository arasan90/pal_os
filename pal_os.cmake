cmake_minimum_required(VERSION 3.10)

set(sources
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/thread.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/queue.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/mutex.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/signal.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/system.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/time.c
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}/timer.c
)

set(public_includes
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(private_includes
    ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM}
)

set(linked_libs
)

function(pal_os_get_sources OUT_VAR)
    set(${OUT_VAR}
        ${sources}
        PARENT_SCOPE)
endfunction()

function(pal_os_get_public_headers OUT_VAR)
    set(${OUT_VAR}
        ${public_includes}
        PARENT_SCOPE)
endfunction()

function(pal_os_get_private_headers OUT_VAR)
    set(${OUT_VAR}
        ${private_includes}
        PARENT_SCOPE)
endfunction()

function(pal_os_get_private_linked_libs OUT_VAR)
    set(${OUT_VAR}
        ${private_linked_libs}
        PARENT_SCOPE)
endfunction()

function(pal_os_get_public_linked_libs OUT_VAR)
    set(${OUT_VAR}
        ${linked_libs}
        PARENT_SCOPE)
endfunction()

function(pal_os_create_mock_library)
    add_library(pal_os_mock ${CMAKE_CURRENT_LIST_DIR}/mock/pal_os_mock.c)
    target_include_directories(pal_os_mock PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    target_include_directories(pal_os_mock PRIVATE ${CMAKE_CURRENT_LIST_DIR}/mock ${CMAKE_CURRENT_LIST_DIR}/src/${TARGET_PLATFORM})
endfunction()