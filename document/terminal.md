# 终端简介
终端是一个独立的设备，看着像一个老式的大头显示器，用户通过终端来与操作系统交互。
终端上用来显示文本，除了文本之外，制造商还希望可以通过非文本（不可见字符）的方式来控制终端执行特定的行为，即控制序列（Contro Sequence），由于没有标准，不同的制造商可以为相同的功能选择不同的控制序列。
后来ECMA协会给终端的控制序列定义了一套标准，这套标准的代号是ECMA-48。

最初是DEC（美国数字设备公司）公司生产的VT100终端，后来升级成VT220，后来又升级成VT525，最后升级成xterm。
升级后的终端都支持更多的控制序列，并且支持ANSI标准的控制序列。

xterm是vt220的超集，xterm支持vt220的所有功能，并且扩展了更多的功能，比如xterm支持显示颜色，但是vt220不支持显示颜色。和xterm一样，VT220在VT100的基础上也扩展了功能。

可以使用linux命令`infocmp \<term1> \<term2>`来查看不同终端控制序列的区别。

# 键盘按键映射

参考：  
* VT100 User Guide/Chapter 3 Programmer Information  
* https://www.cnblogs.com/fanweisheng/p/11076987.html
* https://www.gnu.org/software/screen/manual/html_node/Control-Sequences.html
* https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
* https://vt100.net/docs/vt510-rm/chapter4.html
* https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
* https://unix.stackexchange.com/questions/473599/how-to-resize-tty-console-width
* http://xtermjs.org/docs/api/vtfeatures

## 字母键
默认情况下，发送小写字母的ASCII码，如果按住了Shift键或者Capslock打开了，那么就发送大写字母的ASCII码

下表列出了字母按键对应的ASCII代码：

| 按键 | Uppercase Code | Lowercase Code  | 按键 | Uppercase Code | Lowercase Code |
| :--- | ---------------| --------------- | ---- | -------------- | -------------- |
| A    | 65             | 97              | N    | 78             | 110            |
| B    | 66             | 98              | O    | 79             | 111            |
| C    | 67             | 99              | P    | 80             | 112            |
| D    | 68             | 100             | Q    | 81             | 113            |
| E    | 69             | 101             | R    | 82             | 114            |
| F    | 70             | 102             | S    | 83             | 115            |
| G    | 71             | 103             | T    | 84             | 116            |
| H    | 72             | 104             | U    | 85             | 117            |
| I    | 73             | 105             | V    | 86             | 118            |
| J    | 74             | 106             | W    | 87             | 119            |
| K    | 75             | 107             | X    | 88             | 120            |
| L    | 76             | 108             | Y    | 89             | 121            |
| M    | 77             | 109             | Z    | 90             | 122            |


## 非字母键
非子母键包括数字键，和一些标点符号键。

非字母键可以生成两种代码。当Shift键松开的时候，生成一种代码；当Shift键被按下的时候，生成另外一种代码。

下表列出了非字母键对应生成的代码表：

| 按键 | Shift键没按下 | Shift键被按下 |
| :--- | ---------------| -------------|
| 1    | 65             | 97           |
| 2    | 66             | 98           |
| 3    | 67             | 99           |
| 4    | 68             | 100          |
| 5    | 69             | 101          |
| 6    | 70             | 102          |
| 7    | 71             | 103          |
| 8    | 72             | 104          |
| 9    | 73             | 105          |
| 0    | 74             | 106          |
| -    | 75             | 107          |
| =    | 76             | 108          |
| \[    | 77             | 109          |
| ;    | 77             | 109          |
| '    | 77             | 109          |
| ,    | 77             | 109          |
| .    | 77             | 109          |
| /    | 77             | 109          |
| \    | 77             | 109          |
| `    | 77             | 109          |


## 功能键
功能键用来发送控制字符，控制字符是不可见字符，控制字符不会显示到屏幕上，当终端收到控制字符时，终端执行对应的动作。

下表列出了功能键对应的控制字符和对应的终端要执行的动作：

| 按键       |数据 | 终端执行的动作 |
| :--------- | --- | -------------|
| RETURN     | 13  | CR           |
| LINEFEED   | 10  | Line Feed           |
| BACKSPACE  | 8   | Backspace           |
| TAB        | 9   | Tab           |
| SPACE BAR  | 32  | 在屏幕上显示一个空格，并擦除之前的内容          |
| ESC        | 27  |           |
| DELETE     | 127 | VT100终端忽略这个按键          |


## CTRL键

## 光标控制键（上下左右键）
键盘上还有4个印有箭头的按键，这4个按键按下的时候传输控制序列。如果终端收到了这些序列，光标会往上下左右移动一个字符。

下表列出了光标控制键对应的控制序列：

| 光标键  |VT52 Mode | ANSI Mode and Cursor Key Mode Reset |ANSI Mode and Cursor Key Mode Set|
| :----- | ------ | ---------|---------|
| Up     | ESC A  | ESC [ A  | ESC O A |
| Down   | ESC B  | ESC [ B  | ESC O B |
| Right  | ESC C  | ESC [ C  | ESC O C |
| Left   | ESC D  | ESC [ D  | ESC O D |



## 辅助键盘（数字小键盘）

辅助键盘上的按键通常传输数字、小数点、负号和逗号的代码，主机可以向终端发出命令把辅助按键设置成应用模式，在应用模式下，每个辅助按键都发送控制序列。VT52/ANSI模式和数字/应用模式四种组合所发送的控制序列如下图所示：

__VT52模式下的辅助键盘表__
| 按键  |NumericMode |ApplicationMode|按键|NumbericMode|ApplicatioMode|
| :-| ------ | ---------|---------|--|--|
| 0 | 0 | ESC ? p | -(dash)   | -(dash)   | ESC ? m |
| 1 | 1 | ESC ? q | ,(comma)  | ,(comma)  | ESC ? l* |
| 2 | 2 | ESC ? r | .(period) | .(period) | ESC ? n |
| 3 | 3 | ESC ? s | ENTER     | 同Return按键 | ESC ? M |
| 4 | 4 | ESC ? t | PF1       | ESC P | ESC P |
| 5 | 5 | ESC ? u | PF2       | ESC Q | ESC Q |
| 6 | 6 | ESC ? v | PF3       | ESC R | ESC R |
| 7 | 7 | ESC ? w | PF4       | ESC S | ESC S |
| 8 | 8 | ESC ? x | | | |
| 9 | 9 | ESC ? y | | | |

__ANSI模式下的辅助键盘表__
| 按键  |NumericMode |ApplicationMode|按键|NumbericMode|ApplicatioMode|
| :-| ------ | ---------|---------|--|--|
| 0 | 0 | ESC O p | -(dash)   | -(dash)   | ESC O m |
| 1 | 1 | ESC O q | ,(comma)  | ,(comma)  | ESC O l* |
| 2 | 2 | ESC O r | .(period) | .(period) | ESC O n |
| 3 | 3 | ESC O s | ENTER     | 同Return按键 | ESC O M |
| 4 | 4 | ESC O t | PF1       | ESC O P | ESC O P |
| 5 | 5 | ESC O u | PF2       | ESC O Q | ESC O Q |
| 6 | 6 | ESC O v | PF3       | ESC O R | ESC O R |
| 7 | 7 | ESC O w | PF4       | ESC O S | ESC O S |
| 8 | 8 | ESC O x | | | |
| 9 | 9 | ESC O y | | | |



# ESC转义序列控制字符

| 转义序列 | 英文缩写 | 描述 |
| :--- | ---------------| -------------|
|ESC c|RIS|重绘屏幕|
|ESC D|IND|换行|
|ESC E|NEL|	新的一行|
|ESC H|HTS|设置当前列为制表位|
|ESC M|RI|翻转换行(Reverse linefeed)|
|ESC Z|DECID|DEC 私有定义|
|ESC 7|DECSC|存储当前状态(光标坐标,属性,字符集)|
|ESC 8|DECRC|恢复上一次储存的状态|
|ESC [|CSI|CSI控制序列开始|
|ESC %|RIS|开始一个字符集选择序列<br/>ESC % @：选择默认字符集(ISO 646 / ISO 8859-1)<br/>ESC % G：选择 UTF-8<br/>ESC % 8：选择 UTF-8(已不用)|
|ESC # 8|DECALN|DEC 屏幕校准测试 - 以E's填充屏幕.|
|ESC(|RIS|开始一个 G0 字符集定义序列<br/>ESC( B：选择默认字符集(ISO 8859-1 mapping)<br/>ESC( 0：选择 vt100 图形映射<br/>ESC( U：选择空映射 - 直接访问字符ROM<br/>ESC( K： 选择用户映射 - 由程序mapscrn(8)加载|
|ESC )||开始一个 G1 字符集定义，后面和G0字符集指令相同|
|ESC >|DECPNM|设置数字小键盘模式（NumericMode）|
|ESC =|DECPAM|设置程序键盘模式（ApplicationMode）|
|ESC ]|OSC|OSC控制序列开始|


# CSI转义序列控制字符

CSI转义序列格式：__ESC [ <n> FinalByte__

CSI 序列的动作由其最后一个字符（Final Byte）决定，\<n\>表示CSI要执行的动作的参数。

| FinalByte | 助记符 | 描述 |FinalByte|助记符|描述|
| :--- | ---------------| --|--|--|--|
|@|ICH|在当前光标位置插入\<n\>个空格<br/>这会将所有现有文本移到右侧<br/>向右溢出屏幕的文本会被删除|a|HPR|光标移动到#标记的右边|
|A|CUU|光标上移\<n>个位置|c|DA|以`I am a VT102'应答 ESC [ ? 6 c:|
|B|CUD|光标下移\<n>个位置|d|VPA|光标移动到当前列指定行|
|C|CUF|光标右移\<n>个位置|e|VPR|光标移动到#标记的下一行|
|D|CUB|光标左移\<n>个位置|f|HVP|光标移动到指定的行和列|
|E|CNL|将光标下移到#指示的行的第一列|g|TBC|默认清除当前位置的制表站<br/>ESC [ 3 g: 删除所有制表站|
|F|CPL|将光标上移到#指示的行的第一列|h|TBC|设置模式(见下文)|
|G|CHA|光标移动到当前行的指定列|l|RM|重置模式(见下文)|
|H|CUP|光标移动到指定行和列(以1行1列为参照)|m|SGR|设置属性(见下文)|
|J|ED|删除(默认从当前光标处到行尾的)内容<br/>ESC [ 1 J: 删除从开始到光标处的内容<br/>ESC [ 2 J: 清空整个屏幕|n|DSR|状态报告(见下文)|
|K|EL|删除行(默认从当前光标处到结尾).<br/>ESC [ 1 K: 删除从行首到当前光标处的内容<br/>ESC [ 2 K: 删除光标所在整行|q|DECLL|设置键盘指示灯<br/>ESC [ 0 q: 熄灭所有指示灯<br/>ESC [ 1 q: 点亮 Scroll Lock 灯<br/>ESC [ 2 q: 点亮 Num Lock 灯<br/>ESC [ 3 q: 点亮 Caps Lock 灯|
|L|IL|将\<n\>行插入光标位置的缓冲区<br/>光标所在的行及其下方的行将向下移动。|r|DECSTBM|设置滚动范围; 参数为首行和末行|
|M|DL|从光标所在行开始，删除\<n\>行|s|?|存储光标位置|
|P|DCH|删除当前光标位置的\<n\>个字符<br/>这会从屏幕右边缘以空格字符移动|u|?|恢复光标位置|
|X|ECH|擦除当前光标位置的\<n\>个字符<br/>方法是使用空格字符覆盖它们|`|HPA|光标移动到当前行指定列|

## CSI - SGR参数

| SGR参数 |描述|SGR参数|描述|SGR参数|描述|
| :--- | ---|--|--|--|--|
|0|所有属性重设为默认值|30|黑色背景|44|蓝色背景|
|1|设置边框|31|红色前景|45|品红背景|
|2|设置亮度减半(用一种颜色模拟另一种颜色)|32|绿色前景|46|孔雀蓝背景|
|4|设置底纹(用一种颜色模拟另一种颜色)<br/>用于模拟的颜色由using ESC ] ...设置|33|棕色前景|47|白色背景|
|5|设置闪烁|34|蓝色前景|49|设置默认背景色|
|7|设置反转视频(reverse video)|35|品红前景|
|10|重设选定映像，显示控制标记，反转元素标记|36|孔雀蓝前景|
|11|选择空映像，设置显示控制标记，重设反转标记|37|白色前景|
|12|选择空映像,设置显示控制标记<br/>重设反转标记<br/>在完成映像表转换之前反转标记引起每一字节的高位反转|38|打开下划线,设置默认前景色|
|21|设置正常亮度(和 ECMA-48 不兼容)|39|关闭下划线,设置默认前景色|
|22|设置正常亮度|40|黑色背景|
|24|关闭下划线|41|黑色背景|
|25|不闪烁|42|绿色背景|
|27|反转视频关闭|43|棕色背景|






# 终端相关的指令

* stty size：查看当前终端的行和列数
* echo $TERM：查看当前终端类型，当一个虚拟终端被打开的时候，虚拟终端会有一个终端类型，存储在TERM环境变量里（使用SSH.Net库的时候，CreateShellStream第一个参数的值）
* tput smam：设置DECAWM模式
* tput rmam：取消设置DECAWM模式（DEC AutoWrapMode）

# 控制序列的测试方法

## DEC PrivateMode的测试方式
1. DECAWM - AutoWrapMode
在一行字符数量超出了该行的最大列数的时候会发生，手动在一行里输入很多字符就会出现
使用tput rmam/smam指令可以动态设置DECAWM模式的启用和禁用

## CSI序列的测试方式
1. EL：按上下键切换历史指令的时候会触发EL指令

