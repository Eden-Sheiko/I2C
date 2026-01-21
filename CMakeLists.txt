cmake_minimum_required(VERSION 3.22.1)
project(i2c C)

set(CMAKE_C_STANDARD 11)

add_executable(i2c main.c
        Inc/i2c_interface.h
        Src/i2c_interface.c
        Inc/i2c_log.h)
