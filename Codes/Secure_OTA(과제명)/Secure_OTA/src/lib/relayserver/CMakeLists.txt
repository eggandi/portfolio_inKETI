find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF_C REQUIRED libprotobuf-c)

SET(TARGET_LIB relayserver)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})


add_library(${TARGET_LIB} SHARED 
                                src/librelayserver.c
)
target_compile_options(${TARGET_LIB} PRIVATE -Wall)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/relayserver/src/)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/memory_allocation/src/)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/parson/src/)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/protobuf/src/)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/lib/include/)

target_link_libraries(${TARGET_LIB} PRIVATE ${PROTOBUF_C_LIBRARIES})
target_link_libraries(${TARGET_LIB} PRIVATE pthread curl memory_allocation protobufabs)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/lib)
add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/lib${TARGET_LIB}.*)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy lib${TARGET_LIB}.* ${OUTPUT_DIR}/ )

set(OUTPUT_INCLUDE_DIR ${PROJECT_ROOT}/output/include/)

file(GLOB HEADER_FILES "${PROJECT_ROOT}/src/*.h")
foreach(HEADER_FILE ${HEADER_FILES})
    add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/include/${HEADER_FILE})
    add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${HEADER_FILE} ${OUTPUT_INCLUDE_DIR}/)
endforeach()

add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${PROJECT_ROOT}/src/lib/include/)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${OUTPUT_INCLUDE_DIR}/)
                  