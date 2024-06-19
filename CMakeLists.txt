
add_executable_m7(coral_in_tree_WS2812B_lib
    src/colors.cc
)

target_link_libraries(coral_in_tree_WS2812B_lib
    libs_base-m7_freertos
)
