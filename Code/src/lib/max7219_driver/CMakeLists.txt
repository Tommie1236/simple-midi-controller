# Code/src/lib/max7219_driver/CMakeLists.txt

add_library(max7219_driver STATIC max7219_driver.c)

target_include_directories(max7219_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(max7219_driver PUBLIC
    pico_stdlib
    hardware_spi
)
