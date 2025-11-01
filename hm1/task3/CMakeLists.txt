cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(MyMathProject)

message(INFO: ${MyMath_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

aux_source_directory(src/ SOURCES)

# add_executable(MyMath main.cpp src/ans_handler.cpp src/my_math.cpp)
add_executable(MyMath main.cpp ${SOURCES})

target_include_directories(MyMath PRIVATE include/)

