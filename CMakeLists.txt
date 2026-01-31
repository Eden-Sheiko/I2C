cmake_minimum_required(VERSION 3.22.1)
project(i2c C)

set(CMAKE_C_STANDARD 11)

# 1. Define the Static Library
# We include the source files and the headers
add_library(i2c_static STATIC
        Src/i2c_interface.c
        Inc/i2c_interface.h
        Inc/i2c_log.h
)

# 2. Tell CMake where to find the header files
# This ensures that anything linking to this lib knows where the .h files are
target_include_directories(i2c_static PUBLIC Inc)

# 3. Define the Executable (your main app)
add_executable(i2c_app Src/main.c)

# 4. Link the library to your executable
target_link_libraries(i2c_app PRIVATE i2c_static)

# Optional: Add your tests/examples as separate executables
add_executable(i2c_test Tests/i2c_tests.c)
target_link_libraries(i2c_test PRIVATE i2c_static)