# CMakeLists


# vim: ff=unix ts=4 sw=4 et ai

cmake_minimum_required(VERSION 3.4)
project(mfgtool VERSION 1.1 LANGUAGES C)

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
if (CMAKE_BUILD_TYPE MATCHES "^(MinSizeRel)|([Rr]elease)$")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
endif()

# 链接选项(剔除未用到的lib/section/...)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed")
if (CMAKE_BUILD_TYPE MATCHES "^(MinSizeRel)|([Rr]elease)$")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-gc-sections")
endif()
#set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed")
set(CMAKE_STATIC_LINKER_FLAGS "-Wl,--as-needed")

# 打印配置
include(CMakeLists.Info.txt)

# =======================================================================
# 项目配置
# =======================================================================

set(MFG_SRC_DIR ../source)                      # 源码根目录
set(MFG_INC_DIR .                               # 头文件搜索目录
                ../source)
set(MFG_LNK_LIB m pthread dl)                   # 引用的库
set(MFG_TARGET mfgtool)

add_definitions(-D LINUX)

# 编译要用到的源文件
aux_source_directory(${MFG_SRC_DIR} MFG_SRC_FILES)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/crypto/sha256.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/drivers/av_driver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/drivers/event_driver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/drivers/light_driver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/drivers/wlan_driver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/receivers/cmd_receiver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/receivers/console_receiver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/receivers/tcp_receiver.c)
list(APPEND MFG_SRC_FILES ${MFG_SRC_DIR}/devices/HOST.c)
list(REMOVE_ITEM MFG_SRC_FILES ${MFG_SRC_DIR}/getopt.c)
list(REMOVE_ITEM MFG_SRC_FILES ${MFG_SRC_DIR}/updater.c) # 有些平台没有curl，默认不加入自动更新程序
message(STATUS "MFG_SRC_FILES = " ${MFG_SRC_FILES})

# 获取git版本号并定义GIT_VERSION变量存储版本号
execute_process(COMMAND uuid
                COMMAND awk "{printf $0}"
                OUTPUT_VARIABLE VERSION)

execute_process(COMMAND date
				COMMAND awk "{printf $0}"
				OUTPUT_VARIABLE BUILD_TIME)

configure_file("version.h.in"
               "version.h")

# 引用不同平台的CMakeLists.txt
if(NOT DEFINED MFG_PLATFORM)
    message(STATUS "USE CMakeLists.HOST.txt")
    include(CMakeLists.HOST.txt)
else()
    message(STATUS "USE CMakeLists.${MFG_PLATFORM}.txt")
    include(CMakeLists.${MFG_PLATFORM}.txt)
endif()

# 编译可执行程序
add_executable(${MFG_TARGET} ${MFG_SRC_FILES})

# 链接库
target_link_libraries(${MFG_TARGET} PRIVATE ${MFG_LNK_LIB})
target_include_directories(${MFG_TARGET} PRIVATE ${MFG_INC_DIR})

# 交叉编译配置
if(DEFINED MFG_SYSROOT)
    message(STATUS "MFG_SYSROOT=${MFG_SYSROOT}")
    SET(CMAKE_FIND_ROOT_PATH ${MFG_SYSROOT})
    SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    SET(CMAKE_SYSROOT ${MFG_SYSROOT})
endif()

if(DEFINED MFG_COMPILER_PREFIX)
    message(STATUS "MFG_COMPILER_PREFIX=${MFG_COMPILER_PREFIX}")
    set(CMAKE_C_COMPILER ${MFG_COMPILER_PREFIX}gcc)
    set(CMAKE_CXX_COMPILER ${MFG_COMPILER_PREFIX}g++)
endif()

# 安装程序
if(DEFINED MFG_EXE_INSTALL_DIR)
    message(STATUS "MFG_EXE_INSTALL_DIR=${MFG_EXE_INSTALL_DIR}")
    install(TARGETS mfgtool
            RUNTIME DESTINATION ${MFG_EXE_INSTALL_DIR})
endif()

IF(NOT DEFINED MFG_SYSROOT AND NOT DEFINED MFG_COMPILER_PREFIX AND DEFINED MFG_TOOLCHAIN_CFG)
    include(${MFG_TOOLCHAIN_CFG})
ENDIF()
