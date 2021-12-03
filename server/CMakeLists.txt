# vim: ff=unix ts=4 sw=4 et ai

cmake_minimum_required(VERSION 3.4)
project(mmsvc VERSION 1.1 LANGUAGES C)

# =======================================================================
# 编译配置
# =======================================================================

# 编译类型(禁用! 通过命令行指定)
#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "MinSizeRel")
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_BUILD_TYPE "RelWithDebInfo")

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-label -Wno-unused-function -D _GNU_SOURCE -fPIC")
#if (CMAKE_BUILD_TYPE MATCHES "^(MinSizeRel)|([Rr]elease)$")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
#endif()

# 打印配置

# =======================================================================
# 项目配置
# =======================================================================

set(MMSVC_SRC_DIR source)                      # 源码根目录
set(MMSVC_INC_DIR source)                      # 头文件搜索目录
set(MMSVC_LNK_LIB m pthread ws2_32)                  # 引用的库
set(MMSVC_TARGET mmsvc)

# 编译要用到的源文件
aux_source_directory(${MMSVC_SRC_DIR} MMSVC_SRC_FILES)
#list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/devices/HOST.c)
message(STATUS "MMSVC_SRC_FILES = " ${MMSVC_SRC_FILES})

# 获取git版本号并定义GIT_VERSION变量存储版本号
#execute_process(COMMAND uuid
#                COMMAND awk "{printf $0}"
#                OUTPUT_VARIABLE VERSION)

#execute_process(COMMAND date
#				COMMAND awk "{printf $0}"
#				OUTPUT_VARIABLE BUILD_TIME)

#configure_file("version.h.in"
#               "version.h")

# 引用不同平台的CMakeLists.txt

# 编译可执行程序
add_executable(${MMSVC_TARGET} ${MMSVC_SRC_FILES})

# 链接库
target_link_libraries(${MMSVC_TARGET} PRIVATE ${MMSVC_LNK_LIB})
target_include_directories(${MMSVC_TARGET} PRIVATE ${MMSVC_INC_DIR})
