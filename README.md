# Platform Abstraction Layer – OS (pal_os)
`pal_os` is a portable C library that provides an abstraction layer for core operating system functionality, simplifying cross-platform development and enabling reusable code.
## ✨ Features
This module includes abstractions for:
- 🧵 **Threads** — Platform-independent thread creation and management
- 🔒 **Mutexes/queues** — Synchronization and inter-task communication (FreeRTOS-style)
- 📶 **Signals/events** — Lightweight mechanisms for asynchronous notification
- ⏱️ **Time management** — Absolute and relative time, delays, time measurement
- ⏲️ **Software timers** — One-shot and periodic timers with callbacks
## 🛠️ Goal
The main goal is to provide a consistent, minimal API for abstracting OS features, enabling:
- Seamless portability between embedded systems using freeRTOS and linux host platform
- Modular and testable code
- Integration in complex projects without heavyweight dependencies
## 📦 Repository structure
```
include/          --> Public headers (e.g., pal_thread.h, pal_fs.h, ...)
src/
├── linux/        --> Linux implementation
└── freeRTOS/     --> freeRTOS-specific implementation
libs
└── fff/          --> External library for unit testing in linux environment
test              --> Unit tests folder
```
## 📌 Application Integration
When using `pal_os` in an embedded application, **the toolchain and dependencies specific to the target must be configured in the application CMake project**, not in this library.
In particular, you must define an interface library named `pal_os_freertos` and link it to the real FreeRTOS implementation (e.g., `idf::freertos` for ESP-IDF):
```cmake
add_library(pal_os_freertos INTERFACE)
target_link_libraries(pal_os_freertos INTERFACE idf::freertos)
```
### ✅ Example: Using `pal_os` with ESP32 and ESP-IDF
Below is a minimal example `CMakeLists.txt` for an application that uses `pal_os` with ESP-IDF:
```cmake
cmake_minimum_required(VERSION 3.25)
set(CMAKE_TOOLCHAIN_FILE $ENV{IDF_PATH}/tools/cmake/toolchain-esp32.cmake)
project(pal_test VERSION 0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
idf_build_process(esp32)
add_definitions(-DESP_PLATFORM)
add_executable(${PROJECT_NAME}.elf modules/main/src/main.c)
# Link pal_os_freertos to the actual FreeRTOS implementation
add_library(pal_os_freertos INTERFACE)
target_link_libraries(pal_os_freertos INTERFACE idf::freertos)
# Link against pal_os and any other required components
target_link_libraries(${PROJECT_NAME}.elf pal_os idf::newlib)
idf_build_executable(${PROJECT_NAME}.elf)
# Add the PAL library
add_subdirectory(libs/pal_os)
```
⚠️ **Note:** This setup ensures the microcontroller-specific configuration is done *at the application level*, keeping `pal_os` generic and reusable.
## 🔧 Build
The library can be built both for host systems and embedded targets:
```sh
cmake -B build -DTARGET_PLATFORM=linux
```
You can set `TARGET_PLATFORM` to `linux` or `freeRTOS`.
To enable unit tests and mocks, add the following options:
```sh
cmake -B build -DTARGET_PLATFORM=linux -DPERFORM_UNIT_TESTS=ON -DCOMPILE_MOCK=ON
```
## 📄 License
This project is licensed under the **GNU General Public License v3.0**.