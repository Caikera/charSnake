cmake_minimum_required(VERSION 3.20)
project(charSnake C)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

add_compile_options(-fno-elide-constructors) #关闭编译器优化

set(CMAKE_CXX_FLAGS “-fno-elide-constructors ${CMAKE_CXX_FLAGS}”)

set(CMAKE_C_STANDARD 11)

add_executable(charSnake main.c ${APP_ICON_RESOURCE_WINDOWS})