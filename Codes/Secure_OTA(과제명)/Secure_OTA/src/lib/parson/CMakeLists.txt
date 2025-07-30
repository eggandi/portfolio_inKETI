SET(TARGET_LIB parson)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_library(${TARGET_LIB} SHARED 
                                src/parson.c
)
target_compile_options(${TARGET_LIB} PRIVATE -Wall)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/parson/src/)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/relayserver/src/)

target_link_libraries(${TARGET_LIB} PRIVATE pthread)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/lib)
add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/lib${TARGET_LIB}.*)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy lib${TARGET_LIB}.* ${OUTPUT_DIR}/ )

set(OUTPUT_INCLUDE_DIR ${PROJECT_ROOT}/output/include/)
set(TARGET_HEADER *.h)
add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${PROJECT_ROOT}/src/lib/include/)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${OUTPUT_INCLUDE_DIR}/)
                  