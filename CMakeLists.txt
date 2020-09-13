
cmake_minimum_required(VERSION 3.9)

project(cconf)

set(PROJECT_NAME cconf)
set(EXAMPLE_NAME cconf-example)

include_directories("./include")

file(GLOB SOURCE_FILES "./src/*c")

add_library(${PROJECT_NAME} ${SOURCE_FILES})

add_executable(${EXAMPLE_NAME} "main.c")
target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME})