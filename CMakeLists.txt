enable_language(ASM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fverbose-asm -save-temps")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fverbose-asm -save-temps")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fverbose-asm -save-temps -mthumb -mcpu=cortex-m7")

add_executable_m7(coral_in_tree_WS2812B_lib
    src/main.cc
    src/gpio_control.s
)

target_link_libraries(coral_in_tree_WS2812B_lib
    libs_base-m7_freertos
)