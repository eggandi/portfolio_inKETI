SET(TARGET_APP remove)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_executable(${TARGET_APP})
target_sources(${TARGET_APP} PRIVATE remove.c )

set(OUTPUT_DIR ${PROJECT_ROOT}/output/include)
add_custom_command(TARGET ${TARGET_APP} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/*.h)
add_custom_command(TARGET ${TARGET_APP} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_ROOT}/src/lib/include/*.h )
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${TARGET_APP})
    
