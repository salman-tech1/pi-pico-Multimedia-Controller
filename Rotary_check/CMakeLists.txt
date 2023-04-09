# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
# Tell CMake where to find the executable source file
add_executable(Rotary_check  main.c)
# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)


target_sources(Rotary_check PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/Rotary.c
        )

target_include_directories(Rotary_check PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})


# Set name of project (as PROJECT_NAME) and C/C++ standards
project(Rotary_check C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()



# Create map/bin/hex/uf2 files
pico_add_extra_outputs(Rotary_check)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(Rotary_check PUBLIC pico_stdlib  )

pico_enable_stdio_usb(Rotary_check 1)