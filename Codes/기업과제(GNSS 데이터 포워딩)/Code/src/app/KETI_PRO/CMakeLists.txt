SET(TARGET_APP keti_pcap)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})
SET(USE_PCAP false)

add_executable(${TARGET_APP})

target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)
target_include_directories(${TARGET_APP} PRIVATE ${TARGET_ROOT}/include)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/depend/v2x-sw/include/kvh1a/)

target_link_options(${TARGET_APP} PRIVATE -Wl,--hash-style=gnu -Wl,--as-needed)
target_link_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/depend/v2x-sw/lib/kvh1a)
target_link_libraries(${TARGET_APP} PRIVATE udev pthread rt gps)
if(USE_PCAP)
    target_link_libraries(${TARGET_APP} PRIVATE pcap)
    target_compile_definitions(${TARGET_APP} PRIVATE _D_PCAP_USED=true)
endif()
target_link_libraries(${TARGET_APP} PRIVATE ltev2x-hal atlkremote_linux_u atlkcv2x atlkcli atlkpoti atlksmx atlktest)
target_link_libraries(${TARGET_APP} PRIVATE ffasn1-dot3-2016 ffasn1-base)


target_sources(${TARGET_APP} PRIVATE    src/main.c
                                        src/gnss.c
                                        src/pro_pcap.c
                                        src/utils.c
                                        src/buffer.c
                                        src/config.c
                                        src/pro_v2x.c
)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin)
add_custom_command(TARGET ${TARGET_APP} PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/${TARGET_APP})
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR}/ )
