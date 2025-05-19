project(pal_osal_mock C CXX)

set(${PROJECT_NAME}_SRC
    src/pal_os_mock.c
)

# Aggiungi il target per il test
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})

# Collega Google Test al tuo test
target_link_libraries(${PROJECT_NAME} pal_os fff)
target_include_directories(${PROJECT_NAME} PRIVATE include)
