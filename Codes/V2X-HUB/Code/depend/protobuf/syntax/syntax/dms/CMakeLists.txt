SET(PROTOBUF_TARGET dms)

MESSAGE(STATUS "${PROTOBUF_TARGET} Protobuf source build.")

# 1) Protobuf 
set(GEN_DIR "${PROTOBUF_ROOT}/src")
set(PGV_PROTO_PATH "${PROTOBUF_ROOT}/depend/protoc-gen-validate")

# 4) protoc + PGV로 C++ 바인딩 + 검증코드 생성
set(GEN_DIR_IS "${GEN_DIR}/${PROTOBUF_TARGET}")

set(PROTO_DIR_IS "${CMAKE_CURRENT_LIST_DIR}/${PROTOBUF_TARGET}")
MESSAGE(STATUS "PROTO_DIR_IS: ${PROTO_DIR_IS}")

file(GLOB_RECURSE TARGET_FILES "${PROTO_DIR_IS}/*.proto")
add_custom_command(
  OUTPUT
    "${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.cc"
    "${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.h"
		"${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.validate.cc"
		"${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.validate.h"
	COMMAND ${CMAKE_COMMAND} -E make_directory "${GEN_DIR_IS}"
  COMMAND protoc
    -I "${CMAKE_CURRENT_LIST_DIR}"
		-I "${PGV_PROTO_PATH}"
		--plugin=protoc-gen-validate=/root/ws_go/go/bin/protoc-gen-validate
    --cpp_out="${GEN_DIR_IS}"
    --validate_out="lang=cc:${GEN_DIR_IS}"
		--experimental_allow_proto3_optional
		${TARGET_FILES}
  DEPENDS "${TARGET_FILES}" 
	COMMENT "Generating C++ ${PROTOBUF_TARGET}.pb from ${PROTOBUF_TARGET}.proto"
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${GEN_DIR_IS})

# 5) 합쳐서 “proto_generated” 타겟으로 묶기
add_custom_target(${PROTOBUF_TARGET}_generated ALL DEPENDS
		"${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.cc"
    "${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.h"
		"${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.validate.cc"
		"${GEN_DIR_IS}/${PROTOBUF_TARGET}.pb.validate.h"
)

