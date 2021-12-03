# Buildroot
buildroot是一个生成linux系统的跨平台的编译框架

# buildroot配置文件
有两种方式修改和创建buildroot配置文件。第一种方式是在buildroot目录下手动修改或创建.config文件，第二种方式是在buildroot目录下执行make menuconfig使用可视化界面修改或创建配置文件。该配置文件是buildroot的全局配置文件。  
当然如果自己手里有一个.config文件，可以自己手动把.config文件拷贝到buildroot根目录，这和使用make menuconfig生成.config文件的行为是一样的。  
当buildroot根目录下存在.config文件后，make menuconfig会自动加载.config脚本里的配置。  
在编译的时候，buildroot会根据配置文件里的配置去执行不同的编译动作，比如要编译的项目，在编译前执行指定的脚本，所有的配置都保存在.config文件里。  
关于.config脚本里的配置项的详细说明，请参考[buildroot make参数]一节。

`BR2_ROOTFS_POST_BUILD_SCRIPT`：指定在编译完所有的软件包之后，生成image文件之前要执行的shell脚本的完整路径  
`BR2_ROOTFS_POST_IMAGE_SCRIPT`：指定在生成image之后要执行的shell脚本的完整路径  

# buildroot工作流程


# 在buildroot里添加软件包
1. 把你的软件包目录放在buildroot/package目录里
2. 在你的软件包目录里创建`Config.in`或`Config.in.host`脚本，脚本里存储项目的元数据信息和依赖关系等信息，该脚本主要用来给make menuconfig使用，buildroot会查找你所配置的所有的Config.in脚本并通过make menuconfig显示出来。当创建完脚本后，你就可以执行make menuconfig，通过界面去控制是否需要编译该项目。脚本的语法和内核使用的Kconfig脚本的语法格式相同（https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt）
3. 在你的软件包目录里创建`{PACKAGE_NAME}.mk`脚本，该脚本里存储了软件包的下载，配置，编译（make，cmake，automake）和安装等信息。脚本的完整名字使用软件包的名字加.mk后缀组成。
4. 可选的，在你的软件包目录里创建`{PACKAGE_NAME}.hash`文件，用来做软件包的版本管理。

# Config.in脚本
在buildroot根目录执行make menuconfig命令时，buildroot会解析package目录下的Config.in脚本并显示所有的软件包列表。可以进入`Target packages`选项看到它们。一个最基本的Config.in文件如下所示：
```
config BR2_PACKAGE_LIBFOO
    bool "libfoo"
    help
      This is a comment that explains what libfoo is. The help text
      should be wrapped.

      http://foosoftware.org/libfoo/
```
写完Config.in脚本后，把Config.in脚本加到`buildroot/package/Config.in`脚本里（buildroot/package/Config.in脚本里存放了要编译的所有项目的Config.in文件，执行make menuconfig时，buildroot会去解析这个脚本，并显示出你配置的所有的软件包）：
````
source "package/libfoo/Config.in"
````
Config.in脚本每一行都以关键字开头，后面可以跟多个参数，除了config关键字，其他每个关键字使用tab键缩进。  
`config`：定义一个新的配置入口，下面几行定义了这个配置的属性。一个Config.in文件可以定义多个config入口。  
`prompt <prompt>`：定义了一个提示符，提示符用来显示给用户看，每个config只能有一个提示符。  
`depends on <expr>`：定义了你的项目所依赖的其他项目，如果有多个依赖项，则使用`&&`分割，也可以写多个depends on关键字。
`select <symbol>`：
`help`：显示帮助信息，help元数据信息的每一行都要使用一个tab键加两个空格键做缩进，每一行help信息最多写62个字符，超过62个字符需要换行，help信息里还要包含一个该项目的url，要使用一个空行把url和文字信息隔开。  

# Config.in.host脚本
没用到

# make脚本（*.mk buildroot manual 64页，51页）
所有关于编译，安装，下载的功能都定义在后缀名为mk的脚本里，这是一个非常重要而且有用的脚本，buildroot全靠它去执行编译，安装等动作，后面的章节里会把后缀名为mk的脚本统一叫做make脚本。
每个make脚本都有一个固定的头部：
````
################################################################################
#
# libfoo
#
################################################################################
````
头部标识了模块名字，模块名字写在井号之间，井号的列数是固定的80个，可以直接把上面的代码拷贝到你的脚本里，把模块名字修改成你的模块名字。头部后面必须跟一个空行，然后写其它命令。

在make脚本里定义变量：
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
变量名字和变量的值使用等于号分开，等于号的左边和右边要有一个空格


define LIBFOO_BUILD_CMDS
$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define LIBFOO_INSTALL_TARGET_CMDS
$(INSTALL) -D -m 0755 $(@D)/libfoo.so* $(TARGET_DIR)/usr/lib
$(INSTALL) -d -m 0755 $(TARGET_DIR)/etc/foo.d
endef

$(eval $(generic-package))
````
详细的元数据和变量信息请参考[buildroot package.mk脚本参数]一节

## ${PACKAGE_NAME}.hash脚本
没用到


# 使用br2-external tree（buildroot manual 30页）
在buildroot外添加的项目叫做br2-external tree。
在make的时候，使用`BR2_EXTERNAL`变量指定要使用的br2-external tree的一个或多个目录，多个目录之间用冒号隔开，例如：
buildroot:~$ make BR2_EXTERNAL=/external-tree/:/external-tree2
使用br2-external tree编译的时候，br2-external tree目录必须包含三个文件：external.desc,external.mk,Config.in

## external.desc
存储br2-external tree的名字和描述信息。名字使用`“name”`关键字开头，描述使用`“desc”`关键字开头，关键字后面要加个冒号，然后写对应的名字和描述。  
Buildroot会把`BR2_EXTERNAL_${name}_PATH`和`BR2_EXTERNAL_${name}_DESC`两个变量分别设置成br2-external tree所在的绝对路径和br2-external tree的描述信息，可以在buildroot的其他配置文件或make文件里引用这两个变量。  
例子：  
````
name:external                       // 可以使用BR2_EXTERNAL_${NAME}_PATH变量得到当前br2-external tree的绝对路径
desc:demo br2-external tree         // 可以使用BR2_EXTERNAL_${NAME}_DESC变量得到当前br2-external tree的描述信息
````

## external.mk
external.mk文件用来包含要编译的你自己的软件包里的mk文件，一般都会写成如下形式：  
````
include $(sort $(wildcard $(BR2_EXTERNAL_${NAME}_PATH)/*.mk))
include $(sort $(wildcard $(BR2_EXTERNAL_${NAME}_PATH)/your_package_path/*.mk))
````

## Config.in
Config.in文件用来包含要在make menuconfig里显示的你自己的软件包里的Config.in文件，一般都会写成如下形式：
````
source "$BR2_EXTERNAL_${NAME}_PATH/package/package1/Config.in"
source "$BR2_EXTERNAL_${NAME}_PATH/package/package2/Config.in"
````
除了这三个文件之外，external tree里可能还包含一些其他的可选内容，比如configs目录，这些内容将在下面介绍。


# 使用非本机编译工具链（buildroot manual 11页 6.1.2）
有三种办法在buildroot里使用其他平台的编译工具链
1. 执行make menuconfig，通过Toolchain选项修改要使用的编译工具链

# buildroot make参数
`BR2_EXTERNAL`：指定br2-external tree的目录，多个目录之间使用冒号隔开  

# package.mk脚本参数
参数名字|函数描述|参数类型|备注
---|:--:|---:|---:
${@D}|指定软件包源码目录|字符串|预定义
${PACKAGE_NAME}_BUILD_CMDS|指定编译软件包的函数|函数|用户定义
${PACKAGE_NAME}_INSTALL_TARGET_CMDS|指定安装软件包的函数|函数|用户定义
${PACKAGE_NAME}_BUILD_CMDS|指定编译软件包的函数|函数|用户定义

