
## CSM(C-ITS Service Module)을 빌드하기 위한 CMakeLists.txt


#
# 빌드 출력 설정
#
set(TARGET_MODULE csm)
set(TARGET_SYNTAX csmredis)


add_executable(${TARGET_MODULE})

#
# 사용자 설정
#

# 프로토버퍼 라이브러리 헤더 및 링크
target_include_directories(${TARGET_MODULE} PRIVATE ${Protobuf_INCLUDE_DIR})
target_link_directories(${TARGET_MODULE} PRIVATE ${Protobuf_LIBRARIES})
file(GLOB PROTOBUF_LIB_FILES "${Protobuf_LIBRARIES}/lib*.so" "${Protobuf_LIBRARIES}/lib*.a")
#링크 예외 항목들
list(FILTER PROTOBUF_LIB_FILES EXCLUDE REGEX "protobuf-lite")

foreach(protobuf_lib ${PROTOBUF_LIB_FILES})
  get_filename_component(LIB_NAME ${protobuf_lib} NAME_WE)
  string(REGEX REPLACE "^lib" "" LIB_NAME ${LIB_NAME})  # lib 접두어 제거
  #message(STATUS "Linking with library: ${LIB_NAME}")
  target_link_libraries(${TARGET_MODULE} PRIVATE ${LIB_NAME})
endforeach()

SET(validate_path ${PROJECT_ROOT}/src/depend/protobuf/syntax/src)

#
# 모듈 빌드
#
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/common/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/protobuf/syntax/src/)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/protobuf/syntax/src/${TARGET_SYNTAX})
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/protobuf/syntax/depend/protoc-gen-validate)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/z/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/re2/include)
target_include_directories(${TARGET_MODULE} PRIVATE /usr/local/include)
target_include_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/hiredis/include)
target_include_directories(${TARGET_MODULE} PRIVATE src)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/z/lib)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/re2/lib)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/output/${TARGET_BUILD})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/output/${TARGET_BUILD}/lib)
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/gpsd/${GPSD_VERSION}/lib/${TARGET_BUILD})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/sidelink/${SIDELINK_LIB_VERSION}/lib/${TARGET_BUILD})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/v2x-sw/lib/${TARGET_PLATFORM})
target_link_directories(${TARGET_MODULE} PRIVATE ${PROJECT_ROOT}/src/depend/hiredis/lib)
target_link_libraries(${TARGET_MODULE} PRIVATE kvhcits kvhrsdpr kvhshm kvhjson kvhlog kvhuds kvhredis)
target_link_libraries(${TARGET_MODULE} PRIVATE ffasn1-j2735-2016 ffasn1-base)
target_link_libraries(${TARGET_MODULE} PRIVATE gps m hiredis crypto pthread rt)
target_link_libraries(${TARGET_MODULE} PRIVATE protobuf_hub_csmredis z re2)
target_link_options(${TARGET_MODULE} PRIVATE -Wl,--hash-style=gnu -Wl,--as-needed)
target_sources(${TARGET_MODULE} PRIVATE
               src/csm.c
               src/csm_dgm_if.c
               src/csm_smem_if.c
               src/redis_if/csm_redis_if.c
							 src/redis_if/csm_redis_pb.cc
               src/csm_input_params.c
							 ${validate_path}/validate/validate.pb.cc
							 )


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