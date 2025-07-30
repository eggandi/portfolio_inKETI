SET(TARGET_APP keti_upload)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_executable(${TARGET_APP})

target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)
target_include_directories(${TARGET_APP} PRIVATE ${TARGET_ROOT}/include)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/output/include)

set(LIB_ROOT ${PROJECT_ROOT}/output/)
file(GLOB_RECURSE LIB_FILES "${LIB_ROOT}/*.so" 
                            )
set(FOUND_LIBS "")
set(LIB_DIRS "")

foreach(LIB_PATH ${LIB_FILES})
    get_filename_component(LIB_NAME ${LIB_PATH} NAME EXT) # 라이브러리 이름 추출
    get_filename_component(LIB_DIR ${LIB_PATH} DIRECTORY)
    string(REGEX REPLACE "^lib" "" LIB_NAME ${LIB_NAME})
    string(REGEX REPLACE "^l" "" LIB_NAME ${LIB_NAME})
    list(APPEND FOUND_LIBS ${LIB_NAME})
    list(APPEND LIB_DIRS ${LIB_DIR})
endforeach()
list(REMOVE_DUPLICATES FOUND_LIBS)
list(REMOVE_DUPLICATES LIB_DIRS)

target_link_directories(${TARGET_APP} PRIVATE ${LIB_DIRS})
target_link_libraries(${TARGET_APP} PRIVATE sed_upload_keti curl)

target_sources(${TARGET_APP} PRIVATE src/keti_upload.c)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin)
#add_custom_command(TARGET ${TARGET_APP} PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/${TARGET_APP})
#add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR}/ )


