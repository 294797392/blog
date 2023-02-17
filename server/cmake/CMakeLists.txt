# vim: ff=unix ts=4 sw=4 et ai

cmake_minimum_required(VERSION 3.4)
project(codelife VERSION 1.1 LANGUAGES C)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-label -Wno-unused-function -D _GNU_SOURCE -fPIC")

# 宏定义
add_definitions(-D ENV_MINGW)     # libY有一些宏定义需要Y_ENV_MINGW这个宏

# =======================================================================
# 项目配置
# =======================================================================

set(CL_SRC_DIR ../source)                         # 源码根目录
set(CL_LNK_LIB m Y ws2_32)                        # 引用的库
set(CL_TARGET websvc)

# 头文件搜索目录
include_directories(../source)
include_directories(../source/FDEvent)
include_directories(../source/FDEventPoll)
include_directories(../external/libY)

# 库搜索目录
# link_directories(../depens/libY/mingw)

# 编译要用到的源文件
list(APPEND CL_SOURCE ${CL_SRC_DIR}/main.c)

# 编译可执行程序
add_executable(${CL_TARGET} ${CL_SOURCE})
target_link_libraries(${CL_TARGET} PRIVATE ${CL_LNK_LIB})
