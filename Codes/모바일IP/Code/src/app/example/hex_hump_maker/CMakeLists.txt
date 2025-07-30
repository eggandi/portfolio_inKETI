set(TARGET_APP hex_dump_maker)
add_executable(${TARGET_APP})

target_include_directories(${TARGET_APP} PRIVATE ./include)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/Common)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/Router)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/Router/lib/iproute2/lib)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/Router/lib/iproute2/include)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/Router/lib/iproute2/include/uapi)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/v2x-sw/include)

target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)

target_link_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/libs/v2x-sw/lib/condor5)


target_sources(${TARGET_APP} PRIVATE 
                                    ./src/hex_dump_maker.c
)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin/)
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR})