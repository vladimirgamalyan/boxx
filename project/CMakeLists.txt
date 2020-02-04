cmake_minimum_required(VERSION 3.15) # 3.15 for msvc static runtime, 3.6 for bundled to android sdk (?)
project(boxx)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

add_subdirectory(src)
