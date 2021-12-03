# make脚本简介（*.mk buildroot manual 64页，51页）
buildroot规定，每一个软件包目录里必须要放一个mk脚本，buildroot通过这个脚本控制编译和安装，下载的流程，这是一个非常重要而且有用的脚本。后面的章节里会把后缀名为mk的脚本统一叫做make脚本。

# 命名规则
* 软件包名字：使用软件包的目录名作为软件包名字
* make脚本名字：使用软件包名字作为make脚本名字
* make脚本变量：使用大写的软件包名字加下划线作为变量的前缀

# make脚本格式
make脚本就是一个makefile，makefile里的所有语法都适用于buildroot的make脚本。buildroot的make脚本里有一个固定的头部：
````
################################################################################
#
# libfoo
#
################################################################################
````
头部标识了软件包名字，软件包名字写在井号之间，井号的列数是固定的80个，可以直接把上面的代码拷贝到你的脚本里，把模块名字修改成你的模块名字。头部后面必须跟一个空行，然后写其它命令。

写完头部之后，要定义软件包的元数据信息和一些基本配置信息：
````
# 版本号信息
PACKAGE_VERSION = 1.0
# 软件包的源码名字
PACKAGE_SOURCE = libfoo-$(LIBFOO_VERSION).tar.gz
# 下载软件包源码的地址
PACKAGE_SITE = http://www.foosoftware.org/download
PACKAGE_LICENSE = GPL-3.0+
# 是否要把编译好的软件包安装到中间目录
LIBFOO_INSTALL_STAGING = YES
PACKAGE_LICENSE_FILES = COPYING
# 软件包的依赖项，buildroot检测到依赖项后，会先编译和安装依赖项，然后再编译你的软件包
PACKAGE_DEPENDENCIES = host-libaaa libbbb
````
元数据是buildroot预定义好的可以控制编译和安装流程的一组变量，常用变量将在下面列出来。  
定义变量的时候，等于号两边要分别用一个空格隔开，等于号不要对齐。  

常用变量（manual.pdf 93页）：
PACKAGE_POST_RSYNC 


# 使用不同的构建方式（buildroot manual 62页）
linux平台（或其他嵌入式平台）有很多构建软件包的方式，比如make，cmake，automake，autotools等等，buildroot可以支持非常多的构建平台，对于不同的构建平台，make脚本的元数据信息有所不用，但大体上是相同的。这一节会介绍如何针对不同的构建平台编写不同的make脚本。

## 使用makefile构建软件包

定义完头部后，你可能需要定义一些变量来存储项目的元数据信息，比如版本号，你的项目的依赖项等等：
````
LIBFOO_VERSION = 1.0
LIBFOO_SOURCE = libfoo-$(LIBFOO_VERSION).tar.gz
LIBFOO_SITE = http://www.foosoftware.org/download
LIBFOO_LICENSE = GPL-3.0+
LIBFOO_LICENSE_FILES = COPYING
LIBFOO_INSTALL_STAGING = YES
LIBFOO_CONFIG_SCRIPTS = libfoo-config
LIBFOO_DEPENDENCIES = host-libaaa libbbb
````
变量名字和变量的值使用等于号分开，等于号的左边和右边要有一个空格，不要对齐等于号（buildroot手册上说的）。

定义完变量后，你可能还需要定义一些函数来执行编译和安装以及其他一些操作：
````
define LIBFOO_BUILD_CMDS
    $(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define LIBFOO_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/libfoo.so* $(TARGET_DIR)/usr/lib
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/etc/foo.d
endef

define LIBFOO_INSTALL_STAGING_CMDS
    $(INSTALL) -D -m 0755 $(@D)/libfoo.so* $(STAGING_DIR)/usr/lib
endef
````
使用`define`和`endef`关键字定义一个函数，和makefile一样，函数里的命令要使用一个tab键缩进。

最后，调用buildroot的内建函数来执行生成操作：
````
$(eval $(generic-package))
````
OK，make脚本写完了，在编译的时候，buildroot会根据你编写的脚本里的指令去执行动作。

## 使用cmake构建软件包


# make脚本预定义的变量和函数
为了方便开发者使用，buildroot在make脚本里预定义了很多非常有用的变量和函数：

# make脚本运行逻辑

$(eval $(generic-package))