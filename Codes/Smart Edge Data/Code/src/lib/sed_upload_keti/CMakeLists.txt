SET(TARGET_LIB sed_upload_keti)
SET(TARGET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_library(${TARGET_LIB} SHARED 
                                src/sed_buffer.c
                                src/sed_json.c
                                src/sed_azure.c
                                src/cJSON/cJSON.c
)
target_compile_options(${TARGET_LIB} PRIVATE -Wall -Werror)
target_include_directories(${TARGET_LIB} PRIVATE ${TARGET_ROOT}/include)
target_include_directories(${TARGET_LIB} PRIVATE ${TARGET_ROOT}/include/cJSON)


set(LIB_AZURE ${PROJECT_ROOT}/depend/azure-sdk-for-c/build/sdk/src/azure)
file(GLOB_RECURSE LIB_FILES_AZURE "${LIB_AZURE}/*.a")
set(LIB_CURL ${PROJECT_ROOT}/depend/curl-8.12.1_1-win64-mingw)
file(GLOB_RECURSE LIB_FILES_CURL "${LIB_CURL}/*.dll")
set(LIB_FILES "${LIB_FILES_CURL};${LIB_FILES_AZURE}") 

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

target_link_directories(${TARGET_LIB} PRIVATE ${LIB_DIRS})
target_link_libraries(${TARGET_LIB} PRIVATE -Wl,--start-group
                                            az_core
                                            az_storage_blobs
                                            az_iot_common
                                            az_iot_hub
                                            az_iot_provisioning
                                            -Wl,--end-group
                                            )
                                            
target_link_libraries(${TARGET_LIB} PRIVATE az_win32 az_curl curl-x64)



#set(OUTPUT_DIR ${PROJECT_ROOT}/output/lib)
#add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${TARGET_LIB}.*)
#add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}${TARGET_LIB}.* ${OUTPUT_DIR}/ )

# set(OUTPUT_HEADER_DIR ${PROJECT_ROOT}/output/include)
# #HEADER FILES CLEAR
# file(GLOB_RECURSE HEADER_FILES "${OUTPUT_HEADER_DIR}/*.h")
# foreach(HEADER_PATH ${HEADER_FILES})
#     add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${HEADER_PATH})
# endforeach()

# #SED_HEADER COPY`
# set(SED_HEADER_PATH ${TARGET_ROOT}/include)
# file(GLOB_RECURSE SED_HEADER_FILES "${SED_HEADER_PATH}/*.h")

# foreach(SED_HEADER_PATH ${SED_HEADER_FILES})
#     string(REGEX REPLACE "^${SED_HEADER_PATH}" "" LIB_NAME ${LIB_NAME})
#     add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SED_HEADER_PATH} ${OUTPUT_DIR}/ )
# endforeach()

#set(OUTPUT_INCLUDE_DIR ${PROJECT_ROOT}/output/include/)
#set(TARGET_HEADER sed_*)
#add_custom_command(TARGET ${TARGET_LIB} PRE_BUILD  COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_INCLUDE_DIR}/${TARGET_HEADER})
#add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_ROOT}/include/${TARGET_HEADER} ${OUTPUT_INCLUDE_DIR}/)

