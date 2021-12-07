# vim: ff=unix ts=4 sw=4 et ai

cmake_minimum_required(VERSION 3.4)
project(mm VERSION 1.1 LANGUAGES C)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-label -Wno-unused-function -D _GNU_SOURCE -fPIC")

# 宏定义
add_definitions(-D Y_ENV_MINGW)     # 有一些宏定义需要Y_ENV_MINGW这个宏

# =======================================================================
# 项目配置
# =======================================================================

set(MM_SRC_DIR ../source)                         # 源码根目录
set(MM_LNK_LIB m ws2_32 Y)                        # 引用的库
set(MM_TARGET mm1)

# 头文件搜索目录
include_directories(../source)
include_directories(../depens)

# 库搜索目录
link_directories(../depens/libY/mingw)

# 编译要用到的源文件
list(APPEND MM_SOURCE ${MM_SRC_DIR}/mm_config.c)
list(APPEND MM_SOURCE ${MM_SRC_DIR}/mm_main.c)

# 获取git版本号并定义GIT_VERSION变量存储版本号
#execute_process(COMMAND uuid
#                COMMAND awk "{printf $0}"
#                OUTPUT_VARIABLE VERSION)

#execute_process(COMMAND date
#				COMMAND awk "{printf $0}"
#				OUTPUT_VARIABLE BUILD_TIME)

#configure_file("version.h.in"
#               "version.h")

# 编译可执行程序
add_executable(${MM_TARGET} ${MM_SOURCE})

# 链接库
target_link_libraries(${MM_TARGET} PRIVATE ${MM_LNK_LIB})
