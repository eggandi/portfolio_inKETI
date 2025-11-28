
## HDEM(HMI Data Exchange Module)을 빌드하기 위한 CMakeLists.txt


#
# 빌드 출력 설정
#
set(TARGET_MODULE hdem)


#
# 사용자 설정
#


#
# 모듈 빌드
#
add_executable(${TARGET_MODULE})
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/common/include)
target_include_directories(${TARGET_MODULE} PRIVATE src)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/output/${TARGET_BUILD})
target_link_libraries(${TARGET_MODULE} PRIVATE kvhjson kvhlog kvhrsdpr kvhshm kvhudp kvhuds kvhq)
target_link_options(${TARGET_MODULE} PRIVATE -Wl,--hash-style=gnu -Wl,--as-needed)
target_sources(${TARGET_MODULE} PRIVATE
               src/hdem.c
               src/hdem_dgm_if.c
               src/hdem_hmi_if.c
               src/hdem_input_params.c
               src/hdem_rsdpr.c
               src/hdem_status_data_trx.c)


#
# 빌드 결과물을 output 디렉토리로 복사
#
set(OUTPUT_DIR ${PROJECT_ROOT}/output/${TARGET_BUILD})
add_custom_command(TARGET ${TARGET_MODULE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                   remove ${OUTPUT_DIR}/${TARGET_MODULE})
add_custom_command(TARGET ${TARGET_MODULE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                   remove ${OUTPUT_DIR}/${TARGET_MODULE}-${VERSION})
add_custom_command(TARGET ${TARGET_MODULE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                   copy ${TARGET_MODULE} ${OUTPUT_DIR}/${TARGET_MODULE})
add_custom_command(TARGET ${TARGET_MODULE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                   copy ${TARGET_MODULE} ${OUTPUT_DIR}/${TARGET_MODULE}-${VERSION})
