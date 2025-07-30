project(MobileIP)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 99) # C 표준
set(CMAKE_VERBOSE_MAKEFILE true) # 컴파일 메시지 출력 활성화
add_compile_options(-fPIC)
add_compile_options(-Wall -W -Wshadow -Wextra -Wno-implicit-fallthrough) 

set(OUTPUT_DIR ./output/)
set(CMAKE_SHARED_LIBRARY_PREFIX "l")
SET(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(TARGET_LIB V2XHO_Common)
add_library(${TARGET_LIB} SHARED ./src/libs/Common/V2XHO_Common.c)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Common)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/lib)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include)
target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include/uapi)
target_compile_options(${TARGET_LIB} PRIVATE -Wall -Werror)
target_link_libraries(${TARGET_LIB} PRIVATE pthread m)
add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy *.so ${OUTPUT_DIR}/libs/)

# set(TARGET_LIB V2XHO_Agent)
# add_library(${TARGET_LIB} SHARED ./src/libs/Common/V2XHO_common.c ./src/libs/Agent/V2XHO_Agent.c 
#                                  ./src/libs/Agent/Advertisements/V2XHO_Agent_Advertisements.c
#                                  ./src/libs/Agent/Registration_Reply/V2XHO_Agent_Registration_Reply.c)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Common)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/lib)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include/uapi)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Agent)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Agent/Advertisements)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Agent/Registration_Reply)
# target_compile_options(${TARGET_LIB} PRIVATE -Wall -Werror)
# target_link_libraries(${TARGET_LIB} PRIVATE pthread m)
# add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy *.so ${OUTPUT_DIR}/libs/)

# set(TARGET_LIB V2XHO_Node)
# add_library(${TARGET_LIB} SHARED ./src/libs/Common/V2XHO_common.c ./src/libs/Node/V2XHO_Node.c
#                                  ./src/libs/Node/Registration_Request/V2XHO_Node_Registration_Request.c
#                                  ./src/libs/Node/Solicitation/V2XHO_Node_Solicitation.c)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Common/)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/lib)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include/uapi)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Node)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Node/Solicitation)
# target_include_directories(${TARGET_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Node/Registration_Request)
# target_compile_options(${TARGET_LIB} PRIVATE -Wall -Werror)
# target_link_libraries(${TARGET_LIB} PRIVATE pthread m)
# add_custom_command(TARGET ${TARGET_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy *.so ${OUTPUT_DIR}/libs/)

SET(_DROUTER_LIB_BUILD 1)
if(_DROUTER_LIB_BUILD)
    add_library(V2XHO_Router_object STATIC ./src/libs/Router/lib/iproute2/lib/ax25_ntop.c 
                                        #./src/libs/Router/lib/iproute2/lib/bpf_glue.c 
                                        #./src/libs/Router/lib/iproute2/lib/bpf_legacy.c 
                                        #./src/libs/Router/lib/iproute2/lib/bpf_libbpf.c 
                                        ./src/libs/Router/lib/iproute2/lib/cg_map.c 
                                        ./src/libs/Router/lib/iproute2/lib/color.c 
                                        ./src/libs/Router/lib/iproute2/lib/coverity_model.c 
                                        ./src/libs/Router/lib/iproute2/lib/exec.c 
                                        ./src/libs/Router/lib/iproute2/lib/fs.c 
                                        #./src/libs/Router/lib/iproute2/lib/inet_proto.c 
                                        ./src/libs/Router/lib/iproute2/lib/json_print.c 
                                        #./src/libs/Router/lib/iproute2/lib/json_print_math.c 
                                        ./src/libs/Router/lib/iproute2/lib/json_writer.c 
                                        ./src/libs/Router/lib/iproute2/lib/libgenl.c 
                                        ./src/libs/Router/lib/iproute2/lib/libnetlink.c 
                                        ./src/libs/Router/lib/iproute2/lib/ll_addr.c 
                                        ./src/libs/Router/lib/iproute2/lib/ll_map.c 
                                        ./src/libs/Router/lib/iproute2/lib/ll_proto.c 
                                        ./src/libs/Router/lib/iproute2/lib/ll_types.c 
                                        #./src/libs/Router/lib/iproute2/lib/mnl_utils.c 
                                        ./src/libs/Router/lib/iproute2/lib/mpls_ntop.c 
                                        ./src/libs/Router/lib/iproute2/lib/mpls_pton.c 
                                        ./src/libs/Router/lib/iproute2/lib/names.c 
                                        ./src/libs/Router/lib/iproute2/lib/namespace.c 
                                        ./src/libs/Router/lib/iproute2/lib/netrom_ntop.c 
                                        ./src/libs/Router/lib/iproute2/lib/ppp_proto.c 
                                        ./src/libs/Router/lib/iproute2/lib/rose_ntop.c 
                                        ./src/libs/Router/lib/iproute2/lib/rt_names.c 
                                        ./src/libs/Router/lib/iproute2/lib/selinux.c 
                                        ./src/libs/Router/lib/iproute2/lib/utils.c 
                                        ./src/libs/Router/lib/iproute2/lib/utils_math.c
                                        )
    target_compile_definitions(V2XHO_Router_object PRIVATE -D_GNU_SOURCE -DHAVE_SETNS -DHAVE_HANDLE_AT -D__HAVE_ARCH_STRLCPY -DHAVE_ELF -DNEED_STRLCPY )
    target_compile_options(V2XHO_Router_object PRIVATE -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wformat=2)
    target_include_directories(V2XHO_Router_object PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/lib)
    target_include_directories(V2XHO_Router_object PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include)
    target_include_directories(V2XHO_Router_object PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include/uapi)
    add_custom_command(TARGET V2XHO_Router_object POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy *.a ${OUTPUT_DIR}/libs/)

    add_compile_options(-Wall -W -Wshadow -Wextra) 
    add_library(V2XHO_Router SHARED ./src/libs/Router/V2XHO_Router.c)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/ip)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/lib)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include)
    target_include_directories(V2XHO_Router PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/lib/iproute2/include/uapi)
    target_compile_options(V2XHO_Router PRIVATE -Wall -Werror)
    target_link_libraries(V2XHO_Router PRIVATE pthread)
    target_link_libraries(V2XHO_Router PRIVATE V2XHO_Router_object)
    add_custom_command(TARGET V2XHO_Router POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy *.so ${OUTPUT_DIR}/libs/) 
       
    if(0)
        add_compile_options(-Wall -W -Wshadow -Wextra) 
        set(TARGET_APP router_test_app)
        add_executable(${TARGET_APP})
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/)
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router)
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/libs/iproute2/ip)
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/libs/iproute2/lib)
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/libs/iproute2/include)
        target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_SOURCE_DIR}/src/libs/Router/libs/iproute2/include/uapi)
        target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)
        target_link_libraries(${TARGET_APP} PRIVATE pthread)
        target_link_libraries(${TARGET_APP} PRIVATE V2XHO_Router_object)
        target_sources(${TARGET_APP} PRIVATE ./src/libs/Router/V2XHO_ROUTER.c)
        add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR}/)
    endif()
endif()

ADD_SUBDIRECTORY(src/app/V2XHO)






