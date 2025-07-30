set(TARGET_APP v2x_handover_test)
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
target_link_libraries(${TARGET_APP} PRIVATE pthread)
target_link_libraries(${TARGET_APP} PRIVATE V2XHO_Common V2XHO_Router)
target_link_libraries(${TARGET_APP} PRIVATE curl zip gps)
target_link_libraries(${TARGET_APP} PRIVATE dot2 dot3)
target_link_libraries(${TARGET_APP} PRIVATE ffasn1-base ffasn1-dot2-2016 ffasn1-dot3-2016 ffasn1-j2735-2016)
target_link_libraries(${TARGET_APP} PRIVATE j29451)
target_link_libraries(${TARGET_APP} PRIVATE lteaccess wlanaccess)

target_sources(${TARGET_APP} PRIVATE 
                                    ./src/v2x_handover_test.c
)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin/)
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR})