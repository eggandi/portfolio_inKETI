
## kvhq 라이브러리를 빌드하기 위한 CMakeLists.txt


#
# 빌드 출력 설정
#
set(TARGET_LIB kvhredis)
set(TARGET_LIB_FILE "lib${TARGET_LIB}.a")



#
# 라이브러리 빌드
#
add_library(${TARGET_LIB} STATIC)

target_include_directories(${TARGET_LIB} PRIVATE /usr/local/include)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/depend/hiredis/include)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/common/include)
target_include_directories(${TARGET_LIB} PRIVATE src)
target_sources(${TARGET_LIB} PRIVATE
              src/kvhredis.c
							src/kvhredis_key.c
							src/kvhredis_rx.c
							src/kvhredis_tx.c
							 )


#
# 라이브러리 빌드 결과물 생성 경로 설정
#
if (NOT BUILD_TEST)
  set(OUTPUT_DIR ${PROJECT_ROOT}/output/${TARGET_BUILD})
else ()
  set(OUTPUT_DIR ${PROJECT_ROOT}/output/${TARGET_BUILD}-debug)
endif ()
set_target_properties(${TARGET_LIB} PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})


#
# 파일명에 버전을 추가한 파일을 새롭게 생성
#
if (NOT BUILD_TEST)
  add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                     copy ${OUTPUT_DIR}/${TARGET_LIB_FILE} ${OUTPUT_DIR}/${TARGET_LIB_FILE}-${VERSION})
endif ()
