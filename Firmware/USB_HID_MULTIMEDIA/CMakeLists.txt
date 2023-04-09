# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
# Tell CMake where to find the executable source file
add_executable(USB_HID_MULTIMEDIA)
# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)


target_sources(USB_HID_MULTIMEDIA PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/usb_mouse_device.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        ${CMAKE_CURRENT_LIST_DIR}/Rotary.c
        )

target_include_directories(USB_HID_MULTIMEDIA PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(USB_HID_MULTIMEDIA C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()



# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(USB_HID_MULTIMEDIA PUBLIC pico_stdlib tinyusb_device tinyusb_board)

