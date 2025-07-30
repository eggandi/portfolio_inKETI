
# Protobuf 패키지 찾기
find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF_C REQUIRED libprotobuf-c)

SET(TARGET_LIB protobufabs)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

# proto 파일의 경로
set(PROTO_FILE ${TARGET_ROOT}/src/protobuf.proto)

# 프로토 파일을 컴파일해서 C/C++ 파일 생성 (생성 경로 명시)
add_custom_command(
    OUTPUT ${TARGET_ROOT}/src/protobuf.pb-c.c ${TARGET_ROOT}/src/protobuf.pb-c.h
    COMMAND protoc-c --c_out=${TARGET_ROOT}/src -I ${TARGET_ROOT}/src ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Running protoc-c on ${PROTO_FILE}"
)

# 생성 작업을 수행하는 별도의 타겟 생성
add_custom_target(proto_generated ALL
    DEPENDS ${TARGET_ROOT}/src/protobuf.pb-c.c ${TARGET_ROOT}/src/protobuf.pb-c.h
)

include_directories(${TARGET_ROOT}/src)
# 라이브러리 생성 시, 생성된 프로토 파일을 소스로 추가
add_library(${TARGET_LIB} SHARED 
    src/protobuf.pb-c.c
    src/protobufabs.c
)

# 라이브러리가 proto_generated 타겟에 의존하도록 설정하여,
# 프로토 파일이 반드시 먼저 생성되도록 함
add_dependencies(${TARGET_LIB} proto_generated)

target_link_libraries(${TARGET_LIB} PRIVATE ${PROTOBUF_C_LIBRARIES})

set(OUTPUT_DIR ${PROJECT_ROOT}/output/lib)
add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/lib${TARGET_LIB}.*)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy lib${TARGET_LIB}.* ${OUTPUT_DIR}/ )

set(OUTPUT_INCLUDE_DIR ${PROJECT_ROOT}/output/include/)
set(TARGET_HEADER *.h)
add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${PROJECT_ROOT}/src/lib/include/)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/src/${TARGET_HEADER} ${OUTPUT_INCLUDE_DIR}/)
                  