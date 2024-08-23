cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(SOURCE_FILES
    main.cpp
    Adafruit_MFRC630.cpp
    Adafruit_MFRC630_consts.c
)

add_executable(MyExecutable ${SOURCE_FILES})

target_include_directories(MyExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
