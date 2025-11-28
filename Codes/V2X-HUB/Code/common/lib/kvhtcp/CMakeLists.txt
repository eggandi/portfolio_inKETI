
## kvhtcp 라이브러리를 빌드하기 위한 CMakeLists.txt


#
# 빌드 출력 설정
#
set(TARGET_LIB kvhtcp)
set(TARGET_LIB_FILE "lib${TARGET_LIB}.a")


#
# 테스트 빌드 시 적용되는 설정
#
if (BUILD_TEST)
  set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
  add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
  add_compile_definitions(_EXPORT_INTERNAL_FUNC_)
  link_libraries(gcov)
endif ()


#
# 라이브러리 빌드
#
add_library(${TARGET_LIB} STATIC)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_ROOT}/src/common/include)
target_include_directories(${TARGET_LIB} PRIVATE src)
target_sources(${TARGET_LIB} PRIVATE src/kvhtcp.c)


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
