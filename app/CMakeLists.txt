cmake_minimum_required(VERSION 3.20.0)

list(APPEND DTS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(led_sensor_app)

zephyr_include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../drivers" drivers)
target_sources(app PRIVATE src/main.cpp)
