
## SMEM(Sidelink Message Exchange Module)을 빌드하기 위한 CMakeLists.txt


#
# 빌드 출력 설정
#
set(TARGET_MODULE smem)


#
# 사용자 설정
#


#
# 모듈 빌드
#
add_executable(${TARGET_MODULE})
target_include_directories(${TARGET_MODULE} PRIVATE /usr/local/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/common/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/sidelink/${SIDELINK_LIB_VERSION}/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/v2x-sw/include/${TARGET_PLATFORM})
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/hiredis/include)
target_include_directories(${TARGET_MODULE} PRIVATE src)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/output/${TARGET_BUILD})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/sidelink/${SIDELINK_LIB_VERSION}/lib/${TARGET_BUILD})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/v2x-sw/lib/${TARGET_PLATFORM})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/hiredis/lib)
target_link_libraries(${TARGET_MODULE} PRIVATE kvhlog kvhuds kvhq kvhredis)
target_link_libraries(${TARGET_MODULE} PRIVATE wlanaccess LLC) # note:: 0.3.16 버전을 링크할 경우, 본 라인이 dot2-2016 링크명령 라인보다 아래에 있으면, smem 실행 시 MKx_Init() Symbol lookup error가 발생한다.
target_link_libraries(${TARGET_MODULE} PRIVATE ffasn1-j2735-2016 ffasn1-base)
target_link_libraries(${TARGET_MODULE} PRIVATE ffasn1-dot2-2021 ffasn1-dot3-2016)
target_link_libraries(${TARGET_MODULE} PRIVATE dot3-2016 dot2-2016 j29451)
target_link_libraries(${TARGET_MODULE} PRIVATE ltev2x-hal atlkremote_linux_u atlkcv2x atlkcli atlkpoti atlksmx atlktest)
target_link_libraries(${TARGET_MODULE} PRIVATE pthread rt gps m hiredis crypto)
target_link_options(${TARGET_MODULE} PRIVATE -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--allow-shlib-undefined)
target_sources(${TARGET_MODULE} PRIVATE
               src/csm_if/smem_csm_if.c
               src/dgm_if/smem_dgm_if.c
							 src/redis_if/smem_redis_if.c
               src/sidelink_if/smem_sidelink_if.c
							 src/ltev2x_if/ltev2x.c
							 src/ltev2x_if/ltev2x_dot2.c
							 src/ltev2x_if/ltev2x_dot3.c
							 src/ltev2x_if/ltev2x_j29451.c
               src/smem.c
               src/smem_input_params.c)


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
