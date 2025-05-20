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

- seamless portability between embedded systems (e.g., FreeRTOS, ESP-IDF) and host platforms (Linux)
- modular and testable code
- integration in complex projects without heavyweight dependencieP

## 📦 Repository structure

```text
Include/          --> Public headers (e.g., pal_thread.h, pal_fs.h, ...)
Src/
├── host/         --> Host implementation (Linux/macOS/Windows)
└── esp32/        --> ESP32-specific implementation (FreeRTOS, ESP-IDF)
Private/          --> Internal headers and private helpers
```

## 🔧 Build

The library can be built both for host systems and embedded targets (e.g., ESP32).
```sh
   cmake -B build -DTARGET_PLATFORM=linux
```

   You can set `TARGET_PLATFORM` to `linux` or `esp32` (default is `linux`).
   To enable unit tests and mocks, add the following options:

```sh
   cmake -B build -DTARGET_PLATFORM=linux -DPERFORM_UNIT_TESTS=ON -DCOMPILE_MOCK=ON
```
For ESP32, make sure you have ESP-IDF installed and configured.
## 📄 License

This project is licensed under the **GNU General Public License v3.0**.