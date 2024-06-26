# Just M7
add_executable_m7(coral_in_tree_WS2812B_lib
    src/main.cc
    M7_EXECUTABLE
    coral_in_tree_WS2812B
)


# add_executable_m7(coral_in_tree_WS2812B_lib
#     src/main.cc
#     M4_EXECUTABLE
#     coral_in_tree_WS2812B_lib
# )
# 
# target_link_libraries(coral_in_tree_WS2812B_lib
#     libs_base-m7_freertos
# )
# 
# add_executable_m4(coral_in_tree_WS2812B_m4
#      src/colors.cc
#      )
# 
# target_link_libraries(coral_in_tree_WS2812B_m4
#     libs_base-m4_freertos
# )