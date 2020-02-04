cmake_minimum_required(VERSION 3.6) # 3.15 for msvc static runtime
project(boxx)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

add_subdirectory(src)
