# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/andy/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(epd154 C CXX ASM)

set(CMAKE_BUILD_TYPE Debug)
IF(CMAKE_BUILD_TYPE MATCHES Debug) 
  message("Building with debug symbols")
ENDIF() 

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
file(GLOB epd154SRC 
  "*.c"
  "*.h"
  "./Fonts/*.c"
  "./Fonts/*.h")

add_executable(epd154 epd154.c ${epd154SRC})

pico_set_program_name(epd154 "epd154")
pico_set_program_version(epd154 "0.1")

pico_enable_stdio_uart(epd154 1)
pico_enable_stdio_usb(epd154 0)

# Add the standard library to the build
target_link_libraries(epd154
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(epd154 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(epd154
        hardware_spi
        )

pico_add_extra_outputs(epd154)

