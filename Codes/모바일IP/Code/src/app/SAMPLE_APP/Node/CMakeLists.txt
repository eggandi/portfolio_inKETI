
set(TARGET_APP mobileip_node_app)
add_executable(${TARGET_APP})

target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Router)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Router/lib/iproute2/lib)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Router/lib/iproute2/include)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Router/lib/iproute2/include/uapi)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Agent)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Agent/Advertisements)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Agent/Registration_Reply)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Node)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Node/Solicitation)
target_include_directories(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/lib/Node/Registration_Request)
target_compile_options(${TARGET_APP} PRIVATE -Wall -Werror)
target_link_libraries(${TARGET_APP} PRIVATE pthread m)
target_link_libraries(${TARGET_APP} PRIVATE V2XHO_Node V2XHO_Router)

target_sources(${TARGET_APP} PRIVATE ${PROJECT_ROOT}/src/app/Node/src/${TARGET_APP}.c)

set(OUTPUT_DIR ${PROJECT_ROOT}/output/bin/)
add_custom_command(TARGET ${TARGET_APP} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_APP} ${OUTPUT_DIR}/)