## protobuf syntax 라이브러리를 빌드하기 위한 CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(PROTOBUF CXX)
set(CMAKE_CXX_STANDARD 17) # C++ 표준 (테스트 빌드용)
set(CMAKE_VERBOSE_MAKEFILE true) # 컴파일 메시지 출력 활성화
add_compile_options(-Wall -W -Wshadow -Wextra) # 공통 컴파일 옵션

# 2) Protobuf (C++ 바인딩 + validate)
SET(Protobuf_INCLUDE_DIR /usr/local/protobuf/include)
SET(Protobuf_LIBRARY /usr/local/protobuf/lib/libprotobuf.so)
SET(Protobuf_PROTOC_EXECUTABLE /usr/local/protobuf/bin/protoc)
find_package(Protobuf REQUIRED)


# 1) Protobuf
message(STATUS "Using Protobuf ${Protobuf_VERSION}")
#
# 빌드 출력 설정
#
set(PROTOBUF protobuf_hub)
set(VERSION "1.0.0") # 버전 설정
#
# 공통 설정
#
set(PROTOBUF_SYNTAX_PATH ${CMAKE_CURRENT_LIST_DIR})
set(PROJECT_ROOT "/root/ws_v2xhub")
set(TARGET_BUILD "kvh1a") # 빌드 대상 플랫폼 (예: kvh1a, kvh1b 등)
set(OUTPUT_DIR ${PROJECT_ROOT}/output/${TARGET_BUILD}/lib)

message(STATUS "OUTPUT_DIR ${OUTPUT_DIR}")
# Protobuf·re2 링크 라이브러리 이름 추출
file(GLOB PROTOBUF_LIB_FILES "${Protobuf_LIBRARIES}/lib*.so" "${Protobuf_LIBRARIES}/lib*.a")
list(FILTER PROTOBUF_LIB_FILES EXCLUDE REGEX "protobuf-lite")
foreach(_lib ${PROTOBUF_LIB_FILES})
  get_filename_component(_name ${_lib} NAME_WE)
  string(REGEX REPLACE "^lib" "" _name ${_name})
  list(APPEND PROTOBUF_LINK_LIBS ${_name})
endforeach()
list(APPEND PROTOBUF_LINK_LIBS re2)   # re2 라이브러리도 공통으로 붙임


#
# syntax 하위 디렉터리별로 개별 라이브러리 생성
#
file(GLOB SUBDIRS RELATIVE "${PROTOBUF_SYNTAX_PATH}/syntax" "${PROTOBUF_SYNTAX_PATH}/syntax/*/")
foreach(sub ${SUBDIRS})
  # 실제 소스가 있는 src/<sub> 디렉터리만 처리
  if(IS_DIRECTORY "${PROTOBUF_SYNTAX_PATH}/src/${sub}")
    # 라이브러리 이름: 예) protobuf_hub_<sub>
    set(LIB_NAME "${PROTOBUF}_${sub}")
    # 해당 디렉터리의 .cc 파일 리스트
    file(GLOB_RECURSE SRC_FILES
      "${PROTOBUF_SYNTAX_PATH}/src/${sub}/*.cc"
    )
	
    # 공유 라이브러리 생성
    add_library(${LIB_NAME} SHARED ${SRC_FILES})

    # include 경로
    target_include_directories(${LIB_NAME} PRIVATE
      ${Protobuf_INCLUDE_DIR}
			${PROTOBUF_SYNTAX_PATH}/src/
      ${PROTOBUF_SYNTAX_PATH}/src/${sub}
      ${PROTOBUF_SYNTAX_PATH}/depend/protoc-gen-validate
      ${PROJECT_ROOT}/src/depend/re2/include
    )

    # 링크
    target_link_directories(${LIB_NAME} PRIVATE ${Protobuf_LIBRARIES} ${PROJECT_ROOT}/src/depend/re2/lib)
    target_link_libraries(${LIB_NAME} PRIVATE ${PROTOBUF_LINK_LIBS})

    # 출력 경로 및 버전 태깅
    set_target_properties(${LIB_NAME} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )

    add_custom_command(TARGET ${LIB_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${OUTPUT_DIR}/lib${LIB_NAME}.so
        ${OUTPUT_DIR}/lib${LIB_NAME}.so-${VERSION}
    )
  endif()
endforeach()


#
# 통합 라이브러리 빌드
#
if(NOT)
	file(GLOB FIND_FILES RELATIVE "${PROTOBUF_SYNTAX_PATH}/syntax/" "${PROTOBUF_SYNTAX_PATH}/syntax/*/")
	set(TARGET_FOLDER "")
	set(TARGET_LIB "${PROTOBUF}")
	set(TARGET_LIB_FILE "lib${TARGET_LIB}.so")
	add_library(${TARGET_LIB} SHARED)

	target_include_directories(${TARGET_LIB} PRIVATE ${Protobuf_INCLUDE_DIR})
	target_include_directories(${TARGET_LIB} PRIVATE ${PROTOBUF_SYNTAX_PATH}/src)
	target_include_directories(${TARGET_LIB} PRIVATE ${PROTOBUF_SYNTAX_PATH}/depend/protoc-gen-validate)
	target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/depend/re2/include)

	target_link_directories(${TARGET_LIB} PRIVATE ${Protobuf_LIBRARIES})
	# Protobuf 라이브러리 링크
	file(GLOB PROTOBUF_LIB_FILES "${Protobuf_LIBRARIES}/lib*.so" "${Protobuf_LIBRARIES}/lib*.a")
	#링크 예외 항목들
	list(FILTER PROTOBUF_LIB_FILES EXCLUDE REGEX "protobuf-lite")
	foreach(protobuf_lib ${PROTOBUF_LIB_FILES})
		get_filename_component(LIB_NAME ${protobuf_lib} NAME_WE)
		string(REGEX REPLACE "^lib" "" LIB_NAME ${LIB_NAME})  # lib 접두어 제거
		target_link_libraries(${TARGET_LIB} PRIVATE ${LIB_NAME})
	endforeach()

	target_sources(${TARGET_LIB} PRIVATE ${PROTOBUF_SYNTAX_PATH}/src/validate/validate.pb.cc)
	target_link_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/depend/re2/lib)
	target_link_libraries(${TARGET_LIB} PRIVATE re2)
	foreach(TARGET ${FIND_FILES})
		if(IS_DIRECTORY "${PROTOBUF_SYNTAX_PATH}/src/${TARGET}")
			SET(PROTOBUF_SRC_FILES "")
			file(GLOB_RECURSE PROTOBUF_SRC_FILES "${PROTOBUF_SYNTAX_PATH}/src/${TARGET}/*.cc")
			target_include_directories(${TARGET_LIB} PRIVATE ${PROTOBUF_SYNTAX_PATH}/src/${TARGET})
			target_sources(${TARGET_LIB} PRIVATE ${PROTOBUF_SRC_FILES})
		endif()
	endforeach()


	#
	# 라이브러리 빌드 결과물 생성 경로 설정
	#
	set(OUTPUT_DIR ${PROJECT_ROOT}/output/${TARGET_BUILD}/lib)
	set_target_properties(${TARGET_LIB} PROPERTIES
												ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
												LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
	#
	# 파일명에 버전을 추가한 파일을 새롭게 생성
	#
	add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
											copy ${OUTPUT_DIR}/${TARGET_LIB_FILE} ${OUTPUT_DIR}/${TARGET_LIB_FILE}-${VERSION})
endif()