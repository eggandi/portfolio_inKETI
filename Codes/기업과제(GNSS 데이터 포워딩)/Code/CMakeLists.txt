project(KETI_PROJECT_20250310)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 99) # C 표준
set(CMAKE_VERBOSE_MAKEFILE true) # 컴파일 메시지 출력 활성화
add_compile_options(-fPIC)
add_compile_options(-Wall -W -Wshadow -Wextra -Wno-implicit-fallthrough) 

if (DEFINED ${_WIN32})
    set(PLATFORM "WIN32")
else()
    set(PLATFORM "LINUX")
endif()

set(OUTPUT_DIR ./output/)
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
SET(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

ADD_COMPILE_OPTIONS(-Wall -W -Wshadow -Wextra) 

ADD_SUBDIRECTORY(src/app/KETI_PRO)

