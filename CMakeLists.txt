# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(tcp_client C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tcp_client tcp_client.c 
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/context.c
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/distpatch.s
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/init.c
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/create.c
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/schedule.c
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/sysTick.c
            ${CMAKE_CURRENT_LIST_DIR}/thread/src/queue.c
            # ${CMAKE_CURRENT_LIST_DIR}/thread/src/tsk_wait.c
            # ${CMAKE_CURRENT_LIST_DIR}/thread/src/tsk_exit.c
            )

pico_set_program_name(tcp_client "tcp_client")
pico_set_program_version(tcp_client "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(tcp_client 0)
pico_enable_stdio_usb(tcp_client 1)

# Add the standard library to the build
target_link_libraries(tcp_client
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background 
        pico_stdlib 
        pico_lwip_http hardware_adc    
        )

# Add the standard include files to the build
target_include_directories(tcp_client PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${CMAKE_CURRENT_LIST_DIR}/thread/include
)

pico_add_extra_outputs(tcp_client)