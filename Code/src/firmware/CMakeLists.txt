# Code/src/firmware/CMakeLists.txt

# add source files
add_executable(firmware
  main.cpp
  usb_descriptors.c
)

# include submodules

target_include_directories(firmware PRIVATE
${CMAKE_CURRENT_LIST_DIR})

# link in libraries (almost every hardware/<blah>.h or pico/<blah>.h is hardware_blah or pico_blah)
target_link_libraries(firmware PRIVATE
    pico_stdlib
	pico_multicore
    hardware_spi
    hardware_gpio
    tinyusb_device
    tinyusb_board
    pico_unique_id
)

# enable usb/uart stdout
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0) 



#add_compile_definitions(PICO_STDIO_USB_ENABLE_CDC=1)



# compiler flags
target_compile_options(firmware PRIVATE 
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    -fno-exceptions 
    -fno-check-new 
    $<$<COMPILE_LANGUAGE:CXX>:-fno-enforce-eh-specs>
    -g 
    -ffunction-sections 
    -fdata-sections 
    -O3
    -funroll-loops 
    -Werror 
    -Wall
)

# preprocessor macros
target_compile_definitions(firmware PRIVATE 
    PICO_HEAP_SIZE=2048
    PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
)

# select linker script
#pico_set_binary_type(firmware copy_to_ram)


# create map/bin/hex file etc.
pico_add_extra_outputs(firmware)
#pico_add_dis_output2(firmware)
  

# linker options
target_link_options(firmware
    PRIVATE "LINKER:--print-memory-usage"
)
