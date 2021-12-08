# vim: ff=unix ts=4 sw=4 et ai

cmake_minimum_required(VERSION 3.4)
project(mm VERSION 1.1 LANGUAGES C)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-label -Wno-unused-function -D _GNU_SOURCE -fPIC")

# 宏定义
add_definitions(-D Y_ENV_MINGW)     # libY有一些宏定义需要Y_ENV_MINGW这个宏

# =======================================================================
# 项目配置
# =======================================================================

set(MM_SRC_DIR ../source)                         # 源码根目录
set(MM_LNK_LIB m Y ws2_32)                        # 引用的库
set(MM_TARGET mmsvc)

# 头文件搜索目录
include_directories(../source)
include_directories(../depens)

# 库搜索目录
link_directories(../depens/libY/mingw)

# 编译要用到的源文件
list(APPEND MM_SOURCE ${MM_SRC_DIR}/mm_config.c)
list(APPEND MM_SOURCE ${MM_SRC_DIR}/mm_main.c)

# 编译可执行程序
add_executable(${MM_TARGET} ${MM_SOURCE})
target_link_libraries(${MM_TARGET} PRIVATE ${MM_LNK_LIB})
