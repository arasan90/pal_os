cmake_minimum_required(VERSION 3.25)

project(pal_os_test_app.elf)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} pal_os)