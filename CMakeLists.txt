cmake_minimum_required(VERSION 3.22.1)
project(i2c C)

set(CMAKE_C_STANDARD 11)

# ==========================================
# 1. CORE LIBRARY CONFIGURATION
# ==========================================
add_library(i2c_static STATIC
        Src/i2c_interface.c
        Inc/i2c_interface.h
        Inc/i2c_log.h
)

# Tell CMake where to find the header files
target_include_directories(i2c_static PUBLIC Inc)

# ==========================================
# 3. EXAMPLES
# ==========================================
# Add an executable for your example code
add_executable(i2c_example Example/example.c)
target_link_libraries(i2c_example PRIVATE i2c_static)