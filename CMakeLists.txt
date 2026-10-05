cmake_minimum_required(VERSION 3.20.0)

list(APPEND EXTRA_DTC_OVERLAY_FILE "/app/app.overlay")

find_package(Zephyr REQUIRED HINTS {ZEPHYR_BASE})
project(zephyr_course)

add_subdirectory(drivers)
target_sources(app PRIVATE app/src/main.cpp)
