project(Smart_Edge_Data)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 99) # C 표준
set(CMAKE_VERBOSE_MAKEFILE true) # 컴파일 메시지 출력 활성화
add_compile_options(-fPIC)
add_compile_options(-Wall -W -Wshadow -Wextra -Wno-implicit-fallthrough) 


set(OUTPUT_DIR ./output/)
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
SET(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

ADD_COMPILE_OPTIONS(-Wall -W -Wshadow -Wextra) 

ADD_SUBDIRECTORY(src/lib/sed_upload_keti)

INCLUDE_DIRECTORIES(output/include/)
ADD_SUBDIRECTORY(src/app/upload)
