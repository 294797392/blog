## 控件的继承层次结构

WPF的控件继承层次结构从底层到高层分别是：
DependencyObject -> Visual -> UIElement -> FrameworkElement -> Control
不同的基类他们分别实现了不同的功能，了解每个基类的功能和他们之间的差异对于我们开发自定义控件会更有帮助
我们在开发自定义控件的时候可以根据自定义控件的功能去选择继承最合适的基类
仔细观察其实WPF的继承层次关系设计的很好，每个类所做的事情都是很专一的。
下面根据我个人的理解分别介绍每个基类所实现的功能

### Visual
1. 坐标转换
2. 命中测试（HitTest）
3. 各元素的层级关系（z-index）管理

### UIElement
UIElement继承自Visual，它在Visual提供的功能的基础上，实现了下面几个主要功能：
1. 增加对键盘和鼠标操作的支持
2. 提供与键盘和鼠标相关的事件给外部模块调用。从UIElement实现了IInputElement接口这一点也可以很容易的看出来
3. 实现了WPF的布局的三个步骤（Measure，Arrange，Render）
元素的渲染就是在UIElement这一层去做的

### FrameworkElement
对UIElement由进行了一层封装，更接近于我们平时所使用的控件
Visual和UIElement这两个基类所做的事情都是对界面相关的一些操作的封装，而FramworkElement这一层，顾名思义，肯定是对WPF框架相关的特性进行封装。
框架元素针对于WPF框架的很多特性（比如绑定，资源管理，样式）进行了封装，是WPF框架实现的核心的类
1. 绑定的支持
2. 样式的支持
3. 资源管理的支持
4. 动画的支持
在做VideoTerminal项目的时候发现了一个问题，FrameworkElement不会响应OnTextInput事件，而Control类才可以触发该事件
看了AvalonEdit的源码，发现AvalonEdit在处理用户输入的字符的时候也是继承的Control类（TextArea）

### Control



