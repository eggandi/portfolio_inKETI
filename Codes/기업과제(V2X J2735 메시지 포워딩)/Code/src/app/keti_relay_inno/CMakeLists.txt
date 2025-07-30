SET(TARGET_APP kRelay)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_executable(${TARGET_APP})
target_compile_definitions(${TARGET_APP} PRIVATE _D_DEBUG_LOG=1)

target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)
target_include_directories(${TARGET_APP} PRIVATE ${TARGET_ROOT}/include)
target_include_directories(${TARGET_APP} PRIVATE ${TARGET_ROOT}/include/v2x)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/depend/v2x-sw/include/kvh1a/)

target_link_options(${TARGET_APP} PRIVATE -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--allow-shlib-undefined)
target_link_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/depend/v2x-sw/lib/kvh1a)


target_link_libraries(${TARGET_APP} PRIVATE pthread rt gps m)
target_link_libraries(${TARGET_APP} PRIVATE dot3-2016 dot2-2016 j29451)
target_link_libraries(${TARGET_APP} PRIVATE ffasn1-dot2-2021 ffasn1-dot3-2016 ffasn1-j2735-2016 ffasn1-base)
target_link_libraries(${TARGET_APP} PRIVATE ltev2x-hal atlkremote_linux_u atlkcv2x atlkcli atlkpoti atlksmx atlktest)

target_sources(${TARGET_APP} PRIVATE    src/relay_main.c
                                        src/relay_gnss.c
                                        src/relay_utils.c
                                        src/relay_config.c
                                        src/v2x/relay_v2x.c
																				src/v2x/relay_v2x_tx_wsm.c
																				src/v2x/relay_v2x_tx_j2735.c
																				src/v2x/relay_v2x_j2735_bsm.c
																				src/v2x/relay_v2x_dot2.c
																				src/v2x/relay_v2x_rx.c
)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin)
add_custom_command(TARGET ${TARGET_APP} PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/${TARGET_APP})
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR}/ )

