SET(TARGET_APP relay_test_app)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_executable(${TARGET_APP})

target_compile_options(${TARGET_APP} PRIVATE -Wall)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/include/)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/memory_allocation/src/)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/relayserver/src/)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/parson/src/)

target_link_libraries(${TARGET_APP} PRIVATE pthread curl memory_allocation relayserver parson)

target_sources(${TARGET_APP} PRIVATE
                src/relay_test_app.c
               )

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin/)
add_custom_command(TARGET ${TARGET_APP} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/${TARGET_APP})
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR})
    
                  