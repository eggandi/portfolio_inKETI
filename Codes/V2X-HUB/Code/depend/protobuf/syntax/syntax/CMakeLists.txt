cmake_minimum_required(VERSION 3.13)
project(PROTOBUF CXX)
set(CMAKE_CXX_STANDARD 17) # C++ 표준 (테스트 빌드용)
set(CMAKE_VERBOSE_MAKEFILE true) # 컴파일 메시지 출력 활성화
add_compile_options(-Wall -W -Wshadow -Wextra) # 공통 컴파일 옵션

if(NOT DEFINED ENV{GOROOT} OR NOT "$ENV{GOROOT}" STREQUAL "${_desired_goroot}")
	set(ENV{GOROOT} "/root/ws_go/go")
	set(ENV{GOPATH} "/root/ws_go/go_pkg")
	set(ENV{GOBIN} "/root/ws_go/go_pkg/bin")
	set(ENV{GOPROXY} "https://proxy.golang.org,direct")
	set(ENV{GOSUMDB} "sum.golang.org")
	set(ENV{PATH} "/root/ws_go/go/bin:$ENV{PATH}")
endif()
message(STATUS "GOROOT: $ENV{GOROOT}")
# 2) Protobuf (C++ 바인딩 + validate)
SET(Protobuf_INCLUDE_DIR /usr/local/protobuf/include)
SET(Protobuf_LIBRARY /usr/local/protobuf/lib/libprotobuf.so)
SET(Protobuf_PROTOC_EXECUTABLE /usr/local/protobuf/bin/protoc)
find_package(Protobuf REQUIRED)

SET(PROTOBUF_ROOT "${CMAKE_CURRENT_BINARY_DIR}/..")
SET(PROTOBUF_SYNTAX_ROOT "${CMAKE_CURRENT_BINARY_DIR}")

MESSAGE(STATUS "PROTOBUF_ROOT: ${PROTOBUF_ROOT}")
MESSAGE(STATUS "PROTOBUF_SYNTAX_ROOT: ${PROTOBUF_SYNTAX_ROOT}")

# 1) validate.proto 먼저 생성
set(PGV_PROTO_PATH "${PROTOBUF_ROOT}/depend/protoc-gen-validate")

add_custom_command(
  OUTPUT "${PROTOBUF_ROOT}/src/validate/validate.pb.cc" "${PROTOBUF_ROOT}/src/validate/validate.pb.h"
  COMMAND protoc 
	-I ${PGV_PROTO_PATH}
	--plugin=protoc-gen-validate=/root/ws_go/go/bin/protoc-gen-validate
	--cpp_out=${PROTOBUF_ROOT}/src
	${PGV_PROTO_PATH}/validate/validate.proto
  DEPENDS ${PGV_PROTO_PATH}/validate/validate.proto
  COMMENT "Generating C++ validate.pb from validate.proto"
)

add_custom_target(
  protobuf_validate ALL DEPENDS
    "${PROTOBUF_ROOT}/src/validate/validate.pb.cc"
    "${PROTOBUF_ROOT}/src/validate/validate.pb.h"
)
# CMakeLists.txt가 있는 폴더를 검색해서 add_subdirectory로 빌드
SET(PROTOBUF_FILES "")
file(GLOB PROTOBUF_FILES "${PROTOBUF_SYNTAX_ROOT}/**/CMakeLists.txt")
SET(PROTOBUF_FOLDER "")
foreach(_f IN LISTS PROTOBUF_FILES)
  get_filename_component(PROTOBUF_FOLDER "${_f}" DIRECTORY)
	MESSAGE(STATUS "add_subdirectory: ${PROTOBUF_FOLDER}")
	add_subdirectory(${PROTOBUF_FOLDER})
endforeach()


